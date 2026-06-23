#include "engine.h"
#include "engine/core/Mesh.h"
#include "engine/lib/vector.h"

#include <cmath>
#include <string_view>

using namespace Engine;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

class Playground : public IScript {
  Entity lowerArm, upperArm, joint, tip, base, xAxis, yAxis;
  BackendBuilder::t_Shader shader;

  float time = 0, deltaTime = 0, speed = 0.2f;

  void Start() override;
  void Update() override;
  void End() override {};
};

void Playground::Start() {
  shader = BackendBuilder::createShader("./playground/shaders/vertex.vert",
                                        "playground/shaders/fragment.frag");

  auto baseMesh = Assets::MeshGenerator::Polygon<6>();
  base.setMesh("Base", baseMesh);
  base.setShader(shader.get());

  Assets::Mesh squareMesh = Assets::MeshGenerator::Square();
  lowerArm.setMesh("Square Mesh", squareMesh);
  lowerArm.setShader(shader.get());
  lowerArm.getMesh()->setAnchor({0.0f, -1.0f, 0.0f});
  lowerArm.setParent(&base);
  lowerArm.getTransform().translation = Math::vec3f(
      static_cast<float>(sin(Math::PI / 6) * cos(Math::PI / 6)),
      static_cast<float>(sin(Math::PI / 6) * sin(Math::PI / 6)), 0.0f);
  lowerArm.getTransform().rotation.z = -Math::PI / 3;
  // Dont forget to capture the address and not copy the return type. Maybe I
  // will fix this in the future.
  // squareTransformHandle = &square.addProperty<Property::Transform>();
  upperArm.setMesh("Square Mesh", squareMesh);
  upperArm.setShader(shader.get());
  upperArm.setParent(&lowerArm);
  upperArm.getMesh()->setAnchor({0.0f, -1.0f, 0.0f});
  upperArm.getTransform().translation = Math::vec3f(0.0f, 2.0f, 0.0f);

  auto jointMesh = Assets::MeshGenerator::Polygon<8>();
  joint.setMesh("Joint Mesh", jointMesh);
  joint.setShader(shader.get());
  joint.setParent(&upperArm);
  joint.getTransform().translation = Math::vec3f(0.0f, 0.0f, 0.0f);

  auto tipMesh = Assets::MeshGenerator::Polygon<3>();
  tip.setMesh("Tip", tipMesh);
  tip.setShader(shader.get());
  tip.setParent(&upperArm);
  tip.getTransform().translation = Math::vec3f(0.0f, 2.0f, 0.0f);
  tip.getTransform().rotation.z = Math::PI / 2;

  lowerArm.getTransform().scale = Math::vec3f(0.5f, 1.5f, 0.0f);
  upperArm.getTransform().scale = Math::vec3f(0.5f, 2.0f, 0.0f);
  joint.getTransform().scale = Math::vec3f(0.5f, 0.5f, 0.0f);

  xAxis.setMesh("X-Axis", squareMesh);
  xAxis.setShader(shader.get());
  xAxis.getTransform().scale = Math::vec3f(10.0f, 0.02f, 0.0f);

  yAxis.setMesh("X-Axis", squareMesh);
  yAxis.setShader(shader.get());
  yAxis.getTransform().scale = Math::vec3f(0.02f, 5.0f, 0.0f);

  speed = 1.0f;
}

void Playground::Update() {

  // write a better time management system.
  deltaTime += executionHandle->getWindowHandle()->getTime() - time;
  time = executionHandle->getWindowHandle()->getTime();

  // You are a sexy man, never forget.
  base.getTransform().translation.x = 3.0 * sin(speed * 1.5f * deltaTime);
  base.getTransform().rotation.z = speed * 2 * deltaTime;
  // Maybe Child rotation can be synced to the parent rotation in the backend,
  // but I am sure there will be situaution where that might cause issue.
  lowerArm.getTransform().rotation.z =
      (-Math::PI / 3) + base.getTransform().rotation.z;
  lowerArm.getTransform().scale.y = 1.5f + 0.5 * sin(speed * 2 * deltaTime);
  upperArm.getTransform().rotation.z = speed * deltaTime;
  tip.getTransform().rotation.z =
      (Math::PI / 2) + upperArm.getTransform().rotation.z;

  // dont set uniforms here, the shader is not yet linked.
  // entity submission calculates the transform matrix on submission, which
  // means that any subsequent changes to the transform parameters after
  // submission will not reflect untill next frame.
  // executionHandle->submitEntity(triangle);
  executionHandle->submitEntity(lowerArm);
  executionHandle->submitEntity(upperArm);
  executionHandle->submitEntity(joint);
  executionHandle->submitEntity(tip);
  executionHandle->submitEntity(base);
  executionHandle->submitEntity(xAxis);
  executionHandle->submitEntity(yAxis);
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
