#include "ClassDeclarations.h"
#include "engine.h"

using namespace Engine;

constexpr int WIDTH = 800;
constexpr int HEIGHT = 600;

int main() {
  BackendBuilder::t_Window window = BackendBuilder::createWindow(
      WIDTH, HEIGHT, "This beautiful Engine of mine.");
  window->create();

  BackendBuilder::t_Renderer renderer = BackendBuilder::createRenderer();
  renderer->init();
  renderer->setViewport(0, 0, WIDTH, HEIGHT);

  Execution engine(*renderer.get(), *window.get());

  engine.injectScript<Arm>();

  engine.Start();
  engine.Update();
  engine.End();

  return 0;
}
