#include "util.hpp"
#include "log.hpp"

namespace ogl
{

std::string getErrorString(GLenum error)
{
  switch(error)
  {
  case GL_NO_ERROR:
    return "No error";
  case GL_INVALID_ENUM:
    return "Invalud enum argument";
  case GL_INVALID_VALUE:
    return "Out of range numeric argument";
  case GL_INVALID_OPERATION:
    return "Invalid operation";
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return "Invalid framebuffer operation";
  case GL_OUT_OF_MEMORY:
    return "Out of memory";
  default:
    return "Invalid error code";
  }
}

bool checkError()
{
  GLenum error = glGetError();
  if (error != GL_NO_ERROR) {
    Log::error(std::string("OpenGL error: ")+getErrorString(error));
    return true;
  }

  return false;
}

} // namespace ogl
