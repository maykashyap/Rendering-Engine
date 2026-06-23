#include "GLFWWindow.h"
#include "glad/glad.h"
#include <stdexcept>

using namespace Engine::Backend;

GLFWWindow::GLFWWindow(int width, int height, std::string_view title)
    : handle(nullptr), width(width), height(height), title(title) {}

GLFWWindow::~GLFWWindow() {
  if (handle != nullptr) {
    glfwDestroyWindow(handle);
    handle = nullptr;
  }
  glfwTerminate();
}

GLFWWindow::GLFWWindow(GLFWWindow &&other) noexcept
    : handle(other.handle), width(other.width), height(other.height),
      title(std::move(other.title)) {
  other.handle = nullptr;
}

GLFWWindow &GLFWWindow::operator=(GLFWWindow &&other) noexcept {
  if (this != &other) {
    if (handle != nullptr)
      glfwDestroyWindow(handle);
    handle = other.handle;
    width = other.width;
    height = other.height;
    title = std::move(other.title);
    other.handle = nullptr;
  }
  return *this;
}

void GLFWWindow::framebuffer_size_callback(GLFWwindow *, int w, int h) {
  glViewport(0, 0, w, h);
}

void GLFWWindow::create() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_WAYLAND);

  // glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

  handle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if (handle == nullptr) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window.");
  }

  glfwMakeContextCurrent(handle);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    glfwDestroyWindow(handle);
    glfwTerminate();
    throw std::runtime_error("Failed to initialize GLAD.");
  }

  glfwSetFramebufferSizeCallback(handle, framebuffer_size_callback);
}

void GLFWWindow::pollEvents() {
  glfwPollEvents();
  if (glfwGetKey(handle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(handle, 1);
}

void GLFWWindow::swapBuffers() { glfwSwapBuffers(handle); }

bool GLFWWindow::shouldClose() const { return glfwWindowShouldClose(handle); }

int GLFWWindow::getWidth() const { return width; }
int GLFWWindow::getHeight() const { return height; }
