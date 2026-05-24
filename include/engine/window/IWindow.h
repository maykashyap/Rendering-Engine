#pragma once

namespace Engine {

class IWindow {
public:
  virtual ~IWindow() = default;

  virtual void create() = 0;
  virtual void pollEvents() = 0;
  virtual void swapBuffers() = 0;
  virtual bool shouldClose() const = 0;
  virtual int getWidth() const = 0;
  virtual int getHeight() const = 0;

  // Non-copyable, movable via interface not needed — factory owns lifetime
  IWindow(const IWindow &) = delete;
  IWindow &operator=(const IWindow &) = delete;

protected:
  IWindow() = default;
};

} // namespace Engine
