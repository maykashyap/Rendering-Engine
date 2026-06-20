#include "engine.h"
#include "engine/core/BackendBuilder.h"
#include "engine/lib/vector.h"
#include "engine/properties/Transform.h"

#include <cstddef>
#include <iostream>
#include <string_view>

using namespace Engine;

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

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

  std::vector<float> triangleVertices = {0.0f,  0.5f,  -1.0f, 0.5f, 1.0f, 0.5f,
                                         0.5f,  -0.5f, -1.0f, 0.5f, 1.0f, 0.5f,
                                         -0.5f, -0.5f, -1.0f, 0.5f, 1.0f, 0.5f};
  // Please always keep in mind the faceculling settings
  // wether it is cw or ccw
  std::vector<uint32_t> triangleIndices = {2, 1, 0};
  Assets::Mesh triangleMesh(triangleVertices, triangleIndices, layout,
                            sizeof(VertexPosCol));

  shader = BackendBuilder::createShader("./playground/shaders/vertex.vert",
                                        "playground/shaders/fragment.frag");

  square.addProperty<Property::ShaderProgram>(shader.get());
  square.addProperty<Property::Transform>();
  square.addProperty<Property::Mesh>("Square Mesh", squareMesh);

  triangle.addProperty<Property::ShaderProgram>(shader.get());
  triangle.addProperty<Property::Transform>();
  triangle.addProperty<Property::Mesh>("Triangle Mesh", triangleMesh);

  speed = 1.0f;
}

void Playground::Update() {

  deltaTime += executionHandle->getWindowHandle()->getTime() - time;
  time = executionHandle->getWindowHandle()->getTime();

  auto squareTransformHandle =
      square.getProperty<Property::Transform>("Transform");
  // You are a sexy man, never forget.
  squareTransformHandle->rotation =
      Math::vec3f(0, speed * deltaTime, speed * deltaTime);

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
