#include "sysutils.hpp"

extern "C" {
#define __STDC_WANT_LIB_EXT1__ 1
#include <string.h>
}

namespace ogl
{

std::string cppStrerror(int errnum)
{
#ifdef __STDC_LIB_EXT1__
  size_t msgSize = strerrorlen_s(errnum) + 1;
  char* msg = new char[msgSize];
  (void) strerror_s(msg, msgSize, errnum);

  std::string result(msg);
  delete[] msg;
  return result;
#else
  return std::string(strerror(errnum));
#endif
}

} // namespace ogl
