#include "interpolation.hpp"
#include "shader.hpp"
#include "transform.hpp"
#include "log.hpp"
#include <glutil/MatrixStack.h>
#include <cstdio>
#include <cmath>

#define ARRAY_COUNT( array ) (sizeof( array ) / (sizeof( array[0] ) * (sizeof( array ) != sizeof(void*) || sizeof( array[0] ) <= sizeof(void*))))

namespace
{

float CalcFrustumScale(float fFovDeg)
{
  const float degToRad = 3.14159f * 2.0f / 360.0f;
  float fFovRad = fFovDeg * degToRad;
  return 1.0f / tan(fFovRad / 2.0f);
}

glm::vec4 Vectorize(const glm::fquat theQuat)
{
  glm::vec4 ret;

  ret.x = theQuat.x;
  ret.y = theQuat.y;
  ret.z = theQuat.z;
  ret.w = theQuat.w;

  return ret;
}

glm::fquat Lerp(const glm::fquat &v0, const glm::fquat &v1, float alpha)
{
  glm::vec4 start = Vectorize(v0);
  glm::vec4 end = Vectorize(v1);
  glm::vec4 interp = glm::mix(start, end, alpha);

  std::printf("alpha: %f, (%f, %f, %f, %f)\n", alpha, interp.w, interp.x, interp.y, interp.z);
  std::fflush(stdout);

  interp = glm::normalize(interp);
  return glm::fquat(interp.w, interp.x, interp.y, interp.z);
}

glm::fquat Slerp(const glm::fquat &v0, const glm::fquat &v1, float alpha)
{
  float dot = glm::dot(v0, v1);

  const float DOT_THRESHOLD = 0.9995f;
  if (dot > DOT_THRESHOLD)
    return Lerp(v0, v1, alpha);

  glm::clamp(dot, -1.0f, 1.0f);
  float theta_0 = std::acos(dot);
  float theta = theta_0*alpha;

  glm::fquat v2 = v1 + -(v0*dot);
  v2 = glm::normalize(v2);

  return v0*std::cos(theta) + v2*std::sin(theta);
}

const float fFrustumScale = CalcFrustumScale(20.0f);

glm::fquat g_Orients[] =
{
  glm::fquat(0.7071f, 0.7071f, 0.0f, 0.0f),
  glm::fquat(0.5f, 0.5f, -0.5f, 0.5f),
  glm::fquat(-0.4895f, -0.7892f, -0.3700f, -0.02514f),
  glm::fquat(0.4895f, 0.7892f, 0.3700f, 0.02514f),

  glm::fquat(0.3840f, -0.1591f, -0.7991f, -0.4344f),
  glm::fquat(0.5537f, 0.5208f, 0.6483f, 0.0410f),
  glm::fquat(0.0f, 0.0f, 1.0f, 0.0f),
};

char g_OrientKeys[] =
{
  'Q',
  'W',
  'E',
  'R',

  'T',
  'Y',
  'U',
};

} // namespace

Interpolation::Interpolation(Window* window) :
  Tutorial(window),
  mProgram(0),
  mPositionAttrib(0),
  mColorAttrib(0),

  mModelToCameraMatrixUnif(0),
  mCameraToClipMatrixUnif(0),
  mBaseColorUnif(0),

  mCameraToClipMatrix(0.0f),
  mOrientation(),
  mShip("assets/orientation/Ship.xml")
{
  InitializeProgram();

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

  mWindow->addKeyCallback(onKeyboard);
}

void Interpolation::onKeyboard(Window* win, int key, int, Window::Action act, short)
{
  if (act == Window::RELEASE) {
    return;
  }

  Interpolation* thisTut = static_cast<Interpolation*>(win->getTutorial());

  switch (key) {
  case 32:
    rlzLog(Log::INFO, "Interpolation: "<< (thisTut->mOrientation.ToggleSlerp() ? "Slerp" : "Lerp"));
    break;
  case 'S':
    rlzLog(Log::INFO, "Interpolation: Shortest path "<< (thisTut->mOrientation.ToggleShortPath() ? "ON" : "OFF"));
    break;
  default:
    for(size_t iOrient = 0; iOrient < ARRAY_COUNT(g_OrientKeys); iOrient++) {
      if(key == g_OrientKeys[iOrient])
        thisTut->ApplyOrientation(iOrient);
    }
  }
}

void Interpolation::InitializeProgram()
{
  mProgram = ogl::makePorgram({
                                ogl::Shader(GL_VERTEX_SHADER, "shaders/orientation/PosColorLocalTransform.vert"),
                                ogl::Shader(GL_FRAGMENT_SHADER, "shaders/orientation/ColorMultUniform.frag")
                              });

  mPositionAttrib = glGetAttribLocation(mProgram, "position");
  mColorAttrib = glGetAttribLocation(mProgram, "color");

  mModelToCameraMatrixUnif = glGetUniformLocation(mProgram, "modelToCameraMatrix");
  mCameraToClipMatrixUnif = glGetUniformLocation(mProgram, "cameraToClipMatrix");
  mBaseColorUnif = glGetUniformLocation(mProgram, "baseColor");

  float fzNear = 1.0f; float fzFar = 600.0f;

  mCameraToClipMatrix[0].x = fFrustumScale;
  mCameraToClipMatrix[1].y = fFrustumScale;
  mCameraToClipMatrix[2].z = (fzFar + fzNear) / (fzNear - fzFar);
  mCameraToClipMatrix[2].w = -1.0f;
  mCameraToClipMatrix[3].z = (2 * fzFar * fzNear) / (fzNear - fzFar);

  glUseProgram(mProgram);
  glUniformMatrix4fv(mCameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
  glUseProgram(0);
}

void Interpolation::ApplyOrientation(int iIndex)
{
  mOrientation.AnimateToOrient(iIndex);
}

void Interpolation::framebufferSizeChanged(int w, int h)
{
  mCameraToClipMatrix[0].x = fFrustumScale * (h / (float)w);
  mCameraToClipMatrix[1].y = fFrustumScale;

  glUseProgram(mProgram);
  glUniformMatrix4fv(mCameraToClipMatrixUnif, 1, GL_FALSE, glm::value_ptr(mCameraToClipMatrix));
  glUseProgram(0);

  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

void Interpolation::renderInternal()
{
  mOrientation.UpdateTime();

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glutil::MatrixStack currMatrix;
  currMatrix.Translate(glm::vec3(0.0f, 0.0f, -200.0f));
  currMatrix.ApplyMatrix(glm::mat4_cast(mOrientation.GetOrient()));

  glUseProgram(mProgram);
  currMatrix.Scale(3.0, 3.0, 3.0);
  currMatrix.RotateX(-90);
  //Set the base color for this object.
  glUniform4f(mBaseColorUnif, 1.0, 1.0, 1.0, 1.0);
  glUniformMatrix4fv(mModelToCameraMatrixUnif, 1, GL_FALSE, glm::value_ptr(currMatrix.Top()));

  mShip.Render("tint");

  glUseProgram(0);
}


glm::fquat Orientation::GetOrient() const
{
  glm::fquat currentOrient = g_Orients[m_ixCurrOrient];

  for (const Animation& anim : m_anims)
  {
    currentOrient = anim.GetOrient(currentOrient, m_bSlerp, m_bShortPath);
  }

  return currentOrient;
}

void Orientation::UpdateTime()
{
  for (std::deque<Animation>::iterator it=m_anims.begin(); it != m_anims.end();)
  {
    // Animation finished ?
    if (it->UpdateTime())
    {
      m_ixCurrOrient = it->GetFinalIx();
      it = m_anims.erase(it);
    }
    else
      it++;
  }
}

void Orientation::AnimateToOrient(int ixDestination)
{
  if(m_ixCurrOrient == ixDestination && m_anims.empty())
    return;

  m_anims.emplace_back();
  m_anims.back().StartAnimation(ixDestination, 1.0f);
}

glm::fquat Orientation::Animation::GetOrient(const glm::fquat& initial, bool bSlerp, bool bShortPath) const
{
  float dotProd = glm::dot(initial, g_Orients[m_ixFinalOrient]);
  glm::fquat initialShort = (dotProd < 0.0f && bShortPath) ? -initial : initial;

  if(bSlerp)
  {
    return Slerp(initialShort, g_Orients[m_ixFinalOrient], m_currTimer.GetAlpha());
  }
  else
  {
    return Lerp(initialShort, g_Orients[m_ixFinalOrient], m_currTimer.GetAlpha());
  }

  return initial;
}

void Orientation::Animation::StartAnimation(int ixDestination, float fDuration)
{
  m_ixFinalOrient = ixDestination;
  m_currTimer = Framework::Timer(Framework::Timer::TT_SINGLE, fDuration);
}
