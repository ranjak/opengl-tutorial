#include "shader.hpp"
#include "log.hpp"
#include "sysutils.hpp"
#include <fstream>
#include <cerrno>
#include <sstream>
#include <memory>

namespace ogl
{

GLuint loadShader(GLenum shaderType, const std::string& filename)
{
  std::ifstream shaderFile(filename);

  if (!shaderFile.is_open()) {
    rlzLog(Log::ERROR, "loadShader: couldn't open shader file \""<<filename<<"\": "<<cppStrerror(errno));
    return 0;
  }

  std::string shaderText(static_cast<const std::stringstream&>(std::stringstream() << shaderFile.rdbuf()).str());

  GLuint shader = glCreateShader(shaderType);
  if (shader == 0) {
    return 0;
  }

  // For some reason I can't get &shaderText.c_str() to work...
  const char* shaderTextCstr = shaderText.c_str();
  glShaderSource(shader, 1, &shaderTextCstr, nullptr);
  glCompileShader(shader);

  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLint infoLogLength;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::unique_ptr<GLchar[]> strInfoLog(new GLchar[infoLogLength]);
    glGetShaderInfoLog(shader, infoLogLength, nullptr, strInfoLog.get());

    std::string strShaderType;
    switch(shaderType)
    {
    case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
    case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
    case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
    }

    Log::error(std::string("loadShader: failed to build ")+strShaderType+" shader "+filename+": "+strInfoLog.get());
  }

  return shader;
}

GLuint makePorgram(std::initializer_list<GLuint> shaders)
{
  GLuint program = glCreateProgram();

  if (program == 0) {
    return 0;
  }

  for (auto it=shaders.begin(); it != shaders.end(); ++it) {
    glAttachShader(program, *it);
  }

  glLinkProgram(program);

  GLint status;
  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    GLint infoLogLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

    std::unique_ptr<GLchar[]> strInfoLog(new GLchar[infoLogLength]);
    glGetProgramInfoLog(program, infoLogLength, nullptr, strInfoLog.get());

    Log::error(std::string("makeProgram: linking failed: ")+strInfoLog.get());
  }

  return program;
}

} // namespace ogl
