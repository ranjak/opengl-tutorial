#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "log.hpp"
#include <utility>

class Tutorial;

class Window
{
public:
  enum Action { PRESS, RELEASE, KEYREPEAT, ACT_UNKNOWN };
  enum KeyMod : short { MOD_SHIFT = 0x0001, MOD_ALT = 0x0002, MOD_CONTROL = 0x0004 };

  using KeyCallback = void (*)(Window*, int key, int scancode, Action, short keyMods);
  using MouseMvtCallback = void (*)(Window*, double x, double y);
  using MouseButtonCallback = void (*)(Window*, int button, int action, int mods);
  using ScrollCallback = void (*)(Window*, double xoffset, double yoffset);

  // Mouse buttons, to be defined in implementation.
  static const int MOUSE_LEFT;
  static const int MOUSE_RIGHT;
  static const int MOUSE_MIDDLE;

public:
  virtual ~Window() = 0;

  virtual void setCloseCallback(void (*callback)(Window*)) = 0;

  virtual void setFramebufferResizeCallback(void (*callback)(Window*, int w, int h)) = 0;

  virtual void makeContextCurrent() = 0;

  virtual void setSwapInterval(int interval) = 0;

  virtual void swapBuffers() = 0;

  virtual std::pair<int, int> getSize() = 0;

  virtual std::pair<int, int> getFramebufferSize() = 0;

  virtual void addKeyCallback(KeyCallback callback) = 0;

  virtual int getModifierKeys() = 0;

  virtual void setMouseMovementCallback(MouseMvtCallback callback) = 0;
  virtual void setMouseButtonCallback(MouseButtonCallback callback) = 0;
  virtual void setScrollCallback(ScrollCallback callback) = 0;

  virtual std::pair<double, double> getCursorPos() = 0;

  void setTutorial(Tutorial* tutorial)
  {
    if (mTutorial) {
      Log::error("Window: Tutorial can be set only once.");
    }
    else {
      mTutorial = tutorial;
    }
  }

  Tutorial* getTutorial() { return mTutorial; }

private:
  Tutorial* mTutorial = nullptr;
};

inline Window::~Window() {}

#endif // WINDOW_HPP
