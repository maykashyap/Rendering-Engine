#include "ClassDeclarations.h"
#include "engine.h"

using namespace Engine;

void Arm::Start() {
  auto shader = BackendBuilder::createShader(
      "./playground/shaders/vertex.vert", "playground/shaders/fragment.frag");

  auto baseMesh = Assets::MeshGenerator::Polygon<6>(Math::PI / 3.0f);
  base.setMesh(baseMesh);
  base.setShader(shader);
  executionHandle->registerEntity(base);

  Assets::Mesh squareMesh = Assets::MeshGenerator::Square();
  lowerArm.setMesh(squareMesh);
  lowerArm.setShader(shader);
  lowerArm.getMesh()->setAnchor({0.0f, -1.0f, 0.0f});
  lowerArm.setParent(&base);
  lowerArm.getTransform().translation =
      Math::vec3f(0.0f, static_cast<float>(cos(Math::PI / 6)), 0.0f);
  lowerArm.getTransform().scale = Math::vec3f(0.5f, 1.5f, 0.0f);
  executionHandle->registerEntity(lowerArm);

  upperArm.setMesh(squareMesh);
  upperArm.setShader(shader);
  upperArm.setParent(&lowerArm);
  upperArm.getMesh()->setAnchor({0.0f, -1.0f, 0.0f});
  upperArm.getTransform().translation = Math::vec3f(0.0f, 2.0f, 0.0f);
  upperArm.getTransform().scale = Math::vec3f(0.5f, 3.0f, 0.0f);
  executionHandle->registerEntity(upperArm);

  auto jointMesh = Assets::MeshGenerator::Polygon<8>(0.0f);
  joint.setMesh(jointMesh);
  joint.setShader(shader);
  joint.setParent(&upperArm);
  joint.getTransform().translation = Math::vec3f(0.0f, 0.0f, 0.0f);
  joint.getTransform().scale = Math::vec3f(0.5f, 0.5f, 0.0f);
  executionHandle->registerEntity(joint);

  auto tipMesh = Assets::MeshGenerator::Polygon<3>(Math::PI / 2.0f);
  tip.setMesh(tipMesh);
  tip.setShader(shader);
  tip.setParent(&upperArm);
  tip.getMesh()->setAnchor({0.0f, -0.5, 0.0f});
  tip.getTransform().translation.y = 2.0f;
  tip.getTransform().scale.y = 1.2f;
  executionHandle->registerEntity(tip);

  xAxis.setMesh(squareMesh);
  xAxis.setShader(shader);
  xAxis.getTransform().scale = Math::vec3f(5.0f, 0.03f, 0.0f);
  executionHandle->registerEntity(xAxis);

  yAxis.setMesh(squareMesh);
  yAxis.setShader(shader);
  yAxis.getTransform().scale = Math::vec3f(0.03f, 5.0f, 0.0f);
  executionHandle->registerEntity(yAxis);

  speed = 1.0f;
}

void Arm::Update() {

  // write a better time management system.
  deltaTime += executionHandle->getWindowHandle()->getTime() - time;
  time = executionHandle->getWindowHandle()->getTime();

  // You are a sexy man, never forget.
  base.getTransform().translation.x = 3.0 * cos(speed * 1.5f * deltaTime);
  base.getTransform().translation.y = 3.0 * sin(speed * 1.5f * deltaTime);
  base.getTransform().rotation.z = speed * 2 * deltaTime;
  // Maybe Child rotation can be synced to the parent rotation in the backend,
  // but I am sure there will be situation where that might cause issue.
  lowerArm.getTransform().rotation.z = base.getTransform().rotation.z;
  lowerArm.getTransform().scale.y = 1.5f + 0.5 * sin(speed * 2 * deltaTime);
  upperArm.getTransform().rotation.z = speed * deltaTime;
  tip.getTransform().rotation.z = upperArm.getTransform().rotation.z;
}
