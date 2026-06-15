#include "engine/core/BackendBuilder.h"
#include "engine/core/Entity.h"
#include "engine/core/Execution.h"
#include "engine/core/Mesh.h"
#include "engine/lib/matrix.h"
#include "engine/lib/vector.h"
#include "engine/properties/MeshProperty.h"
#include "engine/properties/ShaderProperty.h"
#include "engine/properties/Transform.h"
#include <cstddef>
#include <iostream>

using namespace Engine;

void log(std::string_view message) { std::cout << message << std::endl; }

class Playground : public IScript {
  Entity square, triangle;

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

  std::vector<float> triangleVertices = {0.0f,  0.5f,  1.0f, 0.5f, 1.0f, 0.5f,
                                         0.5f,  -0.5f, 1.0f, 0.5f, 1.0f, 0.5f,
                                         -0.5f, -0.5f, 1.0f, 0.5f, 1.0f, 0.5f};
  // Please always keep in mind the faceculling settings
  // wether it is cw or ccw
  std::vector<uint32_t> triangleIndices = {2, 1, 0};
  Assets::Mesh triangleMesh(triangleVertices, triangleIndices, layout,
                            sizeof(VertexPosCol));

  square.addProperty<Property::ShaderProgram>(
      "./playground/shaders/vertex.vert", "playground/shaders/fragment.frag");
  square.addProperty<Property::Transform>((Math::mat4x4f)0);
  square.addProperty<Property::Mesh>("Square Mesh", squareMesh);

  triangle.addProperty<Property::ShaderProgram>(
      "./playground/shaders/vertex.vert", "playground/shaders/fragment.frag");
  triangle.addProperty<Property::Transform>((Math::mat4x4f)0);
  triangle.addProperty<Property::Mesh>("Triangle Mesh", triangleMesh);
}

void Playground::Update() {
  executionHandle->submitEntity(triangle);
  executionHandle->submitEntity(square);
}

int main() {
  BackendBuilder::t_Window window =
      BackendBuilder::createWindow(800, 600, "This beautiful Engine of mine.");
  window->create();

  BackendBuilder::t_Renderer renderer = BackendBuilder::createRenderer();
  renderer->init();
  renderer->setViewport(0, 0, 800, 600);

  Execution engine(*renderer.get(), *window.get());

  IScript::executionHandle = &engine;

  engine.pushScript(new Playground());

  engine.Start();
  engine.Update();
  engine.End();

  return 0;
}
