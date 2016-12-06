#include "renderer.hpp"
#include "window.hpp"
#include "log.hpp"
#include "tutorial.hpp"
#include "sysutils.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cassert>

namespace
{
// Whether OpenGL extensions have already been loaded
bool initDone = false;

// Hack to allow casting function pointer to void*
// http://stackoverflow.com/questions/16026183/gcc-warns-about-function-pointer-to-object-pointer-cast
template <typename RESULT, typename ...ARGS>
void * make_void_ptr(RESULT (*p)(ARGS...))
{
    static_assert(sizeof(void *) == sizeof(void (*)(void)),
                  "object pointer and function pointer sizes must equal");
    void *q = &p;
    return *static_cast<void **>(q);
}

GLADloadproc myglGetProcAddress = [](const char* proc)
{
  return make_void_ptr(glfwGetProcAddress(proc));
};

void resizeFramebuffer(int w, int h)
{
  // Keep a square (1:1) aspect ratio
  int size = ogl::min(w, h);

  glViewport(w/2 - size/2, h/2 - size/2, size, size);
}

/*
 * Temporary tutorial stuff
 */
GLuint vao = 0;

} // namespace

Renderer::Renderer(Window* window) :
  mWindow(window)
{
  mWindow->makeContextCurrent();

  // Load OpenGL extensions only once.
  // Theoretically they should be loaded for every context,
  // but in practice we don't need to care (glad doesn't care anyway).
  if (!initDone) {
    gladLoadGLLoader(myglGetProcAddress);

    initDone = true;
  }

  glGenVertexArrays(1, &vao);

  std::pair<int, int> framebuffer = mWindow->getFramebufferSize();
  resizeFramebuffer(framebuffer.first, framebuffer.second);

  mWindow->setFramebufferResizeCallback([](Window* /*win*/, int w, int h)
  {
    resizeFramebuffer(w, h);
  });
}

Renderer::~Renderer()
{

}

void Renderer::render(Tutorial& tutorial)
{
  mWindow->makeContextCurrent();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(vao);

  tutorial.render();

  mWindow->swapBuffers();
}
