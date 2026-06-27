#include "ClassDeclarations.h"
#include "engine.h"

using namespace Engine;

void Cube::Start() {
  auto shader =
      BackendBuilder::createShader("playground/shaders/vertex/vertex2d.vert",
                                   "playground/shaders/fragment/fragment.frag");
  auto cubeMesh = Assets::MeshGenerator::Square();
  cube.setMesh(cubeMesh);
  cube.setShader(shader);
  executionHandle->registerEntity(cube);

  executionHandle->attachCamera(camera.addProperty<Property::CameraProperty>());
  camera.getTransform().translation.z = -1;
  float width = executionHandle->getWindowHandle()->getWidth();
  float height = executionHandle->getWindowHandle()->getHeight();
  camera.getProperty<Property::CameraProperty>("Camera")->setOrthographic(
      0.0f, 8, 6, 0.0f, -10.0f, 100.0f);
}

void Cube::Update() {}
