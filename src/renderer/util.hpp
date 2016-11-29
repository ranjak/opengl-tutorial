#ifndef UTIL_HPP
#define UTIL_HPP

#include "log.hpp"
#include <glad/glad.h>
#include <string>

namespace ogl
{

std::string getErrorString(GLenum error);

bool checkError();

} // namespace ogl

#define OGL_CHECK_ERROR() \
  do { GLenum error = glGetError(); \
       if (error != GL_NO_ERROR) \
         rlzLog(Log::ERROR, "OpenGL error ("<<__FILE__<<","<<__LINE__<<"): "<<ogl::getErrorString(error)); \
  } while (0)


#endif // UTIL_HPP
