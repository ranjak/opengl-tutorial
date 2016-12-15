#ifndef DEPTHBUFFERING_HPP
#define DEPTHBUFFERING_HPP

#include "basevertex.hpp"

class DepthBuffering : public BaseVertex
{
public:
  DepthBuffering(Window* window);

private:
  void renderInternal() override;
};

#endif // DEPTHBUFFERING_HPP
