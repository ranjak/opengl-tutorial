#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <string>
#include <initializer_list>

namespace ogl
{

GLuint loadShader(GLenum shaderType, const std::string &filename);

GLuint makePorgram(std::initializer_list<GLuint> shaders);

} // namespace ogl

#endif // SHADER_HPP
