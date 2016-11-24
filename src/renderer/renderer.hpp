#ifndef RENDERER_HPP
#define RENDERER_HPP

class Window;

class Renderer
{
public:
  Renderer(Window* window);
  ~Renderer();

  void render();

  void addTriangle();

private:
  Window* mWindow;
};

#endif // RENDERER_HPP
