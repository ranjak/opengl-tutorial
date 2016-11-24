#include "renderer.hpp"
#include "window.hpp"
#include "primitives.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

/*
 * Temporary tutorial stuff
 */
GLuint bufferObject = 0;
//GLuint program = 0;

} // namespace

Renderer::Renderer(Window* window) :
  mWindow(window)
{
  // Load OpenGL extensions only once.
  // Theoretically they should be loaded for every context,
  // but in practice we don't need to care (glad doesn't care anyway).
  if (!initDone) {
    mWindow->makeContextCurrent();

    gladLoadGLLoader(myglGetProcAddress);

    initDone = true;
  }

  std::pair<int, int> framebuffer = mWindow->getFramebufferSize();
  glViewport(0, 0, framebuffer.first, framebuffer.second);

  mWindow->setFramebufferResizeCallback([](Window* /*win*/, int w, int h)
  {
    glViewport(0, 0, w, h);
  });
}

Renderer::~Renderer()
{

}

void Renderer::render()
{
  mWindow->makeContextCurrent();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
/*
  glUseProgram(program);

  glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glDrawArrays(GL_TRIANGLES, 0, 3);

  glDisableVertexAttribArray(0);
  glUseProgram(0);
*/
  mWindow->swapBuffers();
}

void Renderer::addTriangle()
{
  mWindow->makeContextCurrent();

  glGenBuffers(1, &bufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::triangle), ogl::triangle, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
