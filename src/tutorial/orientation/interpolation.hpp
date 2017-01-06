#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

#include "tutorial.hpp"
#include "window.hpp"
#include "Mesh.h"
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

  glm::fquat GetOrient() const
  {
    if(m_bIsAnimating)
      return m_anim.GetOrient(g_Orients[m_ixCurrOrient], m_bSlerp);
    else
      return g_Orients[m_ixCurrOrient];
  }

  bool IsAnimating() const {return m_bIsAnimating;}

  void UpdateTime()
  {
    if(m_bIsAnimating)
    {
      bool bIsFinished = m_anim.UpdateTime();
      if(bIsFinished)
      {
        m_bIsAnimating = false;
        m_ixCurrOrient = m_anim.GetFinalIx();
      }
    }
  }

  void AnimateToOrient(int ixDestination)
  {
    if(m_ixCurrOrient == ixDestination)
      return;

    m_anim.StartAnimation(ixDestination, 1.0f);
    m_bIsAnimating = true;
  }

private:
  class Animation
  {
  public:
    //Returns true if the animation is over.
    bool UpdateTime()
    {
      return m_currTimer.Update();
    }

    glm::fquat GetOrient(const glm::fquat &initial, bool bSlerp) const
    {
      if(bSlerp)
      {
        return Slerp(initial, g_Orients[m_ixFinalOrient], m_currTimer.GetAlpha());
      }
      else
      {
        return Lerp(initial, g_Orients[m_ixFinalOrient], m_currTimer.GetAlpha());
      }

      return initial;
    }

    void StartAnimation(int ixDestination, float fDuration)
    {
      m_ixFinalOrient = ixDestination;
      m_currTimer = Framework::Timer(Framework::Timer::TT_SINGLE, fDuration);
    }

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

Orientation g_orient;

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
  void OffsetOrientation(const glm::vec3& _axis, float fAngDeg);

private:
  GLuint mProgram;
  GLuint mPositionAttrib;
  GLuint mColorAttrib;

  GLuint mModelToCameraMatrixUnif;
  GLuint mCameraToClipMatrixUnif;
  GLuint mBaseColorUnif;

  glm::mat4 mCameraToClipMatrix;
  glm::fquat mOrientation;

  Framework::Mesh mShip;
};

#endif // INTERPOLATION_HPP
