#ifndef RENDERER_HPP
#define RENDERER_HPP

class Window;
class Tutorial;

class Renderer
{
public:
  Renderer(Window* window);
  ~Renderer();

  void render(Tutorial& tutorial);

private:
  Window* mWindow;
};

#endif // RENDERER_HPP
