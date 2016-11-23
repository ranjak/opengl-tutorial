#ifndef RENDERER_HPP
#define RENDERER_HPP

class Window;

class Renderer
{
public:
  Renderer(Window* window);

  void render();

private:
  Window* mWindow;
};

#endif // RENDERER_HPP
