#include "OpenGLRenderer.h"
#include "engine/core/IRenderer.h"
#include "engine/lib/math/matrix.h"
#include <algorithm>
#include <cstdint>
#include <iostream>

using namespace Engine::Backend;

// Comments needed.
OpenGLGPUVAHandle::OpenGLGPUVAHandle(const Assets::Mesh &mesh) {
  m_indexCount = mesh.getIndexCount();

  glGenVertexArrays(1, &m_vaoID);
  glBindVertexArray(m_vaoID);

  glGenBuffers(1, &m_vboID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
  // this function requires size, but in this case we can use the .size() which
  // returns the count because we are using raw binary vector with a type of
  // uint8
  glBufferData(GL_ARRAY_BUFFER, mesh.get_vertex_bytes().size(),
               mesh.get_vertex_bytes().data(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_eboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount * sizeof(uint32_t),
               mesh.get_indices().data(), GL_STATIC_DRAW);

  const auto &layout = mesh.getLayout();
  size_t stride = mesh.getVertexSize();

  for (const auto &component : layout) {
    GLuint shaderLocation = static_cast<GLuint>(component.component);

    glEnableVertexAttribArray(shaderLocation);

    glVertexAttribPointer(shaderLocation, static_cast<GLint>(component.format),
                          GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride),
                          reinterpret_cast<const void *>(
                              static_cast<uintptr_t>(component.offset)));
  }
  glBindVertexArray(0);
}

OpenGLGPUVAHandle::~OpenGLGPUVAHandle() {
  if (m_vboID)
    glDeleteBuffers(1, &m_vboID);
  if (m_eboID)
    glDeleteBuffers(1, &m_eboID);
  if (m_vaoID)
    glDeleteVertexArrays(1, &m_vaoID);
}

// move

void OpenGLGPUVAHandle::bind() const { glBindVertexArray(m_vaoID); }
void OpenGLGPUVAHandle::unbind() const { glBindVertexArray(0); }

// ___________________________________________________________________

void OpenGLRenderer::init() {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  // logging

  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Forces OpenGL to wait until the
                                         // error prints

  // 2. Define the callback function
  glDebugMessageCallback(
      [](GLenum, GLenum, GLuint, GLenum severity, GLsizei,
         const GLchar *message, const void *) {
        if (severity == GL_DEBUG_SEVERITY_HIGH ||
            severity == GL_DEBUG_SEVERITY_MEDIUM) {
          std::cerr << "[OpenGL Error] " << message << std::endl;
        }
      },
      nullptr);
}

void OpenGLRenderer::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
             static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenGLRenderer::clear(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderer::sceneStart(const Math::mat4x4f &viewmat,
                                const Math::mat4x4f &projmat) {
  m_viewProjection = projmat * viewmat;
}

void OpenGLRenderer::submit(const Renderer::RendererCommand &command) {
  m_renderQueue.push_back(command);
}

void OpenGLRenderer::sceneEnd() {
  if (m_renderQueue.empty()) {
    return;
  }

  std::sort(
      m_renderQueue.begin(), m_renderQueue.end(),
      [](const Renderer::RendererCommand &a,
         const Renderer::RendererCommand &b) { return a.shader < b.shader; });

  flush();
  m_renderQueue.clear();
}

#include <iomanip>
#include <iostream>
void PrintMatrixEveryFrame(const Engine::Math::mat4x4f &matrix) {
  static bool first_frame = true;
  if (!first_frame) {
    std::cout << "\033[4A";
  }
  first_frame = false;
  std::cout << std::fixed << std::setprecision(3);
  for (std::size_t r = 0; r < 4; ++r) {
    std::cout << "\r| ";

    for (std::size_t c = 0; c < 4; ++c) {
      std::cout << std::setw(9) << matrix(r, c) << " ";
    }

    std::cout << "|\033[K\n";
  }
  std::cout << std::flush;
}
void OpenGLRenderer::flush() {
  const Assets::IShader *shaderCurrent = nullptr;
  PrintMatrixEveryFrame(m_viewProjection);
  for (const auto &command : m_renderQueue) {
    if (shaderCurrent != command.shader) {
      command.shader->use();
      shaderCurrent = command.shader;
      command.shader->setUniform("m_viewProjection", &m_viewProjection);
    }
    // here you assign the shader mat4 uniform the translation matrix
    command.shader->setUniform("v_anchor", command.anchor);
    command.shader->setUniform("m_transform",
                               command.translation->getTransformMatrix());
    command.vaHandle->bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(command.vaHandle->getIndexCount()),
                   GL_UNSIGNED_INT, 0);
  }
  glBindVertexArray(0);
}
