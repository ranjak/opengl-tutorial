#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include "Mesh.h"
#include "framework/Timer.h"
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>


class Orientation
{
public:
  Orientation()
    : m_bIsAnimating(false)
    , m_ixCurrOrient(0)
    , m_bSlerp(false)
  {}

  bool ToggleSlerp()
  {
    m_bSlerp = !m_bSlerp;
    return m_bSlerp;
  }

  glm::fquat GetOrient() const;

  bool IsAnimating() const {return m_bIsAnimating;}

  void UpdateTime();

  void AnimateToOrient(int ixDestination);

private:
  class Animation
  {
  public:
    //Returns true if the animation is over.
    bool UpdateTime()
    {
      return m_currTimer.Update();
    }

    glm::fquat GetOrient(const glm::fquat &initial, bool bSlerp) const;

    void StartAnimation(int ixDestination, float fDuration);

    int GetFinalIx() const {return m_ixFinalOrient;}

  private:
    int m_ixFinalOrient;
    Framework::Timer m_currTimer;
  };

  bool m_bIsAnimating;
  int m_ixCurrOrient;
  bool m_bSlerp;

  Animation m_anim;
};

class Animation
{
public:

private:
  glm::fquat m_finalOrient;
};


class Interpolation : public Tutorial
{
public:
  Interpolation(Window* window);

private:
  static void onKeyboard(Window* win, int key, int, Window::KeyAction act, short /*mods*/);

  void framebufferSizeChanged(int w, int h) override;
  void renderInternal() override;

  void InitializeProgram();
  void ApplyOrientation(int iIndex);

private:
  GLuint mProgram;
  GLuint mPositionAttrib;
  GLuint mColorAttrib;

  GLuint mModelToCameraMatrixUnif;
  GLuint mCameraToClipMatrixUnif;
  GLuint mBaseColorUnif;

  glm::mat4 mCameraToClipMatrix;
  Orientation mOrientation;

  Framework::Mesh mShip;
};

#endif // INTERPOLATION_HPP
