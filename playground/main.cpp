#include "engine.h"
#include "engine/core/BackendBuilder.h"
#include "engine/lib/vector.h"
#include "engine/properties/Transform.h"

#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string_view>

using namespace Engine;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

void PrintMatrixEveryFrame(const Engine::Math::mat4x4f &matrix) {
  static bool first_frame = true;

  if (!first_frame) {
    // 🟢 ANSI Escape Code: Move the terminal cursor UP 4 lines
    std::cout << "\033[4A";
  }
  first_frame = false;

  // Force floating point layout with a fixed precision
  std::cout << std::fixed << std::setprecision(3);

  for (std::size_t r = 0; r < 4; ++r) {
    // Slam cursor to start of line, print opening bracket
    std::cout << "\r| ";

    for (std::size_t c = 0; c < 4; ++c) {
      // std::setw(9) ensures every number takes exactly 9 characters of space.
      // This keeps columns aligned even if a number changes from 0.000 to
      // -125.451
      std::cout << std::setw(9) << matrix(r, c) << " ";
    }

    // 🟢 ANSI Escape Code: \033[K clears the rest of the line to the right.
    // This acts as the "ghost character eraser" if numbers shrink.
    std::cout << "|\033[K\n";
  }

  // Force the terminal to draw the entire block immediately
  std::cout << std::flush;
}
void PrintMatrix(const Math::mat4x4f &matrix) {
  // Set fixed floating-point format with 3 decimal places
  std::cout << std::fixed << std::setprecision(3);

  std::cout << "Matrix 4x4:\n";
  for (std::size_t r = 0; r < 4; ++r) {
    std::cout << "| ";
    for (std::size_t c = 0; c < 4; ++c) {
      // Keeps columns perfectly aligned regardless of number size
      std::cout << std::setw(9) << matrix(r, c) << " ";
    }
    std::cout << "|\n";
  }
  std::cout << std::endl; // Flushes the stream and adds a final spacing line
}

class Playground : public IScript {
  Entity square, triangle;
  BackendBuilder::t_Shader shader;

  float time = 0, deltaTime = 0, speed = 0.2f;

  void Start() override;
  void Update() override;
  void End() override {};
};

void Playground::Start() {
  struct VertexPosCol {
    Math::vec3f position;
    Math::vec3f color;
  };
  Assets::VertexLayout layout = {
      {Assets::ComponentTypes::POSITION, Assets::ComponentSize::Float3,
       offsetof(VertexPosCol, position)},
      {Assets::ComponentTypes::COLOR, Assets::ComponentSize::Float3,
       offsetof(VertexPosCol, color)}};

  // Mesh Asset
  std::vector<float> squareVertices = {
      // Vertex           // Colors
      0.5f,  0.5f,  0.0f, 0.3, 0.5, 0.9, // top right
      0.5f,  -0.5f, 0.0f, 0.3, 0.5, 0.9, // bottom right
      -0.5f, -0.5f, 0.0f, 0.3, 0.5, 0.9, // bottom left
      -0.5f, 0.5f,  0.0f, 0.3, 0.5, 0.9  // top left
  };
  std::vector<uint32_t> squareIndices = {0, 2, 1, 0, 3, 2};
  Assets::Mesh squareMesh(squareVertices, squareIndices, layout,
                          sizeof(VertexPosCol));

  std::vector<float> triangleVertices = {0.0f,  0.5f,  0.0f, 0.5f, 1.0f, 0.5f,
                                         0.5f,  -0.5f, 0.0f, 0.5f, 1.0f, 0.5f,
                                         -0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f};
  // Please always keep in mind the faceculling settings
  // wether it is cw or ccw
  std::vector<uint32_t> triangleIndices = {2, 1, 0};
  Assets::Mesh triangleMesh(triangleVertices, triangleIndices, layout,
                            sizeof(VertexPosCol));

  shader = BackendBuilder::createShader("./playground/shaders/vertex.vert",
                                        "playground/shaders/fragment.frag");

  square.addProperty<Property::ShaderProgram>(shader.get());
  auto &squareTransformHandle = square.addProperty<Property::Transform>();
  square.addProperty<Property::Mesh>("Square Mesh", squareMesh);

  triangle.addProperty<Property::ShaderProgram>(shader.get());
  triangle.addProperty<Property::Transform>();
  triangle.addProperty<Property::Mesh>("Triangle Mesh", triangleMesh);

  squareTransformHandle.scale = (Math::vec3f)0.4;
  squareTransformHandle.rotation = Math::vec3f(0, 0, Math::PI / 4);

  speed = 1.0f;
}

void Playground::Update() {

  deltaTime += executionHandle->getWindowHandle()->getTime() - time;
  time = executionHandle->getWindowHandle()->getTime();

  // You are a sexy man, never forget.
  auto squareTransformHandle =
      square.getProperty<Property::Transform>("Transform");
  squareTransformHandle->rotation = Math::vec3f(0, 0, speed * deltaTime);
  squareTransformHandle->translation = Math::vec3f(
      0.5 * cos(speed * deltaTime), 0.5 * sin(speed * deltaTime), 0);

  // dont set uniforms here, the shader is not yet linked.
  // entity submission calculates the transform matrix on submission, which
  // means that any subsequent changes to the transform parameters after
  // submission will not reflect untill next frame.
  // executionHandle->submitEntity(triangle);
  executionHandle->submitEntity(square);
}

int main() {
  BackendBuilder::t_Window window = BackendBuilder::createWindow(
      WIDTH, HEIGHT, "This beautiful Engine of mine.");
  window->create();

  BackendBuilder::t_Renderer renderer = BackendBuilder::createRenderer();
  renderer->init();
  renderer->setViewport(0, 0, WIDTH, HEIGHT);

  Execution engine(*renderer.get(), *window.get());

  IScript::executionHandle = &engine;

  engine.injectScript<Playground>();

  engine.Start();
  engine.Update();
  engine.End();

  return 0;
}
