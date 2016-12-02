#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <initializer_list>

namespace ogl
{

class Shader
{
public:
  Shader(GLenum shaderType, const std::string &filename);
  ~Shader();

  GLuint getHandle() const { return mHandle; }

private:
  GLuint mHandle;
};

GLuint makePorgram(std::initializer_list<Shader> shaders);

} // namespace ogl

#endif // SHADER_HPP
