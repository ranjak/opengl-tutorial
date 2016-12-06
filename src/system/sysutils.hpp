#ifndef SYSUTILS_HPP
#define SYSUTILS_HPP

#include <string>

namespace ogl
{

std::string cppStrerror(int errnum);

// Redefine here to avoid including <algorithm> just for min/max
template<typename T>
T min(const T& a, const T& b)
{
  return (a < b) ? a : b;
}

template<typename T>
T max(const T& a, const T& b)
{
  return (a > b) ? a : b;
}

} // namespace ogl

#endif // SYSUTILS_HPP
