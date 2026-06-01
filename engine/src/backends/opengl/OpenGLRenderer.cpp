#include "OpenGLRenderer.h"
#include <algorithm>
#include <cstdint>

using namespace Engine::Backend;

// Comments needed.
OpenGLGPUVAHandle::OpenGLGPUVAHandle(const Assets::Mesh &mesh) {
  m_indexCount = mesh.getIndexCount();

  glGenVertexArrays(1, &m_vaoID);
  glBindVertexArray(m_vaoID);

  glGenBuffers(1, &m_vboID);
  glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
  glBufferData(GL_ARRAY_BUFFER, mesh.get_vertex_bytes().size(),
               mesh.get_vertex_bytes().data(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_eboID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.get_indices().size(),
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
}

void OpenGLRenderer::setViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
  glViewport(static_cast<GLint>(x), static_cast<GLint>(y),
             static_cast<GLsizei>(width), static_cast<GLsizei>(height));
}

void OpenGLRenderer::frameStart() {
  // assign matrix
  m_renderQueue.clear();
}

void OpenGLRenderer::submit(const Renderer::RendererCommand &command) {
  m_renderQueue.push_back(command);
}

void OpenGLRenderer::frameEnd() {
  if (m_renderQueue.empty())
    return;

  std::sort(m_renderQueue.begin(), m_renderQueue.end(),
            [](const Renderer::RendererCommand &a,
               const Renderer::RendererCommand &b) {
              return a.shader.get() < b.shader.get();
            });

  flush();
}

void OpenGLRenderer::flush() {
  IShader *shaderCurrent = nullptr;

  for (const auto &command : m_renderQueue) {
    if (shaderCurrent != command.shader.get()) {
      command.shader->use();
      shaderCurrent = command.shader.get();
      // you would also assign the shader mat4 uniform your own projection
      // matrix here
    }
    // here you assign the shader mat4 uniform the translation matrix
    command.vaHandle->bind();
    glDrawElements(GL_TRIANGLES,
                   static_cast<GLsizei>(command.vaHandle->getIndexCount()),
                   GL_UNSIGNED_INT, nullptr);
  }
  glBindVertexArray(0);
}
