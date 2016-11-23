#ifndef WINDOW_HPP
#define WINDOW_HPP

class Window
{
public:
  virtual ~Window() = 0;

  virtual void setCloseCallback(void (*callback)(Window*)) = 0;

  virtual void makeContextCurrent() = 0;

  virtual void setSwapInterval(int interval) = 0;

  virtual void swapBuffers() = 0;
};

inline Window::~Window() {}

#endif // WINDOW_HPP
