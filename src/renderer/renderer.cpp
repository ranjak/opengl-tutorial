#include "renderer.hpp"
#include "window.hpp"
#include "primitives.hpp"
#include "shader.hpp"
#include "log.hpp"
#include "util.hpp"
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

/*
 * Temporary tutorial stuff
 */
GLuint bufferObject = 0;
GLuint program = 0;
GLuint vao = 0;

GLint windowHeightUniformLoc = 0;

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

  glGenVertexArrays(1, &vao);

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

  glBindVertexArray(vao);

  glUseProgram(program);
  OGL_CHECK_ERROR();

  glUniform1i(windowHeightUniformLoc, mWindow->getFramebufferSize().second);

  glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
  OGL_CHECK_ERROR();
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
  OGL_CHECK_ERROR();
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnableVertexAttribArray(0);
  OGL_CHECK_ERROR();

  glDrawArrays(GL_TRIANGLES, 0, 3);
  OGL_CHECK_ERROR();

  glDisableVertexAttribArray(0);
  glUseProgram(0);
  OGL_CHECK_ERROR();

  mWindow->swapBuffers();
}

void Renderer::addTriangle()
{
  mWindow->makeContextCurrent();

  glGenBuffers(1, &bufferObject);

  glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(ogl::triangle), ogl::triangle, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  program = ogl::makePorgram({
                     ogl::Shader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl"),
                     ogl::Shader(GL_VERTEX_SHADER, "shaders/vertex.glsl")
                   });

  windowHeightUniformLoc = glGetUniformLocation(program, "windowHeight");
  assert(windowHeightUniformLoc != -1);

  // TODO delete shaders
}
