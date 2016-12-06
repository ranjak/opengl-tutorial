#ifndef TUTORIAL_HPP
#define TUTORIAL_HPP


class Tutorial
{
public:
  virtual ~Tutorial() = 0;

  virtual void init() {}

  virtual void update() {}

  virtual void render() {}
};

inline Tutorial::~Tutorial() {}

#endif // TUTORIAL_HPP
