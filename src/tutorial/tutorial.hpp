#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP

#include "system.hpp"

class Tutorial
{
public:
  virtual ~Tutorial() = 0;

  virtual void init() {}

  virtual void update(ogl::seconds /*delta*/) {}

  virtual void render() {}
};

inline Tutorial::~Tutorial() {}

#endif // TUTORIAL_HPP
