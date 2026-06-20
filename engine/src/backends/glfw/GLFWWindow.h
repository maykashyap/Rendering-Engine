#pragma once
#include "engine/window/IWindow.h"
#include <GLFW/glfw3.h>
#include <string>
#include <string_view>

namespace Engine::Backend {

class GLFWWindow final : public IWindow {
private:
  GLFWwindow *handle = nullptr;
  int width;
  int height;
  std::string title;

  static void framebuffer_size_callback(GLFWwindow *window, int w, int h);

public:
  GLFWWindow(int width, int height, std::string_view title);
  ~GLFWWindow() override;

  GLFWWindow(GLFWWindow &&other) noexcept;
  GLFWWindow &operator=(GLFWWindow &&other) noexcept;

  void create() override;
  void pollEvents() override;
  void swapBuffers() override;

  bool shouldClose() const override;
  int getWidth() const override;
  int getHeight() const override;

  double getTime() const override { return glfwGetTime(); }
};

} // namespace Engine::Backend
