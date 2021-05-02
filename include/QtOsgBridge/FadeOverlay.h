#pragma once

#include <osgHelper/Animation.h>

#include <QtOsgBridge/VirtualOverlay.h>

namespace QtOsgBridge
{

class FadeOverlay : public VirtualOverlay
{
public:
  enum class FadeMode
  {
    In,
    Out
  };

  FadeOverlay(float duration = 0.5f, bool isFadedOutInitially = false);
  ~FadeOverlay() override;

  FadeMode getCurrentFadeMode() const;

  void fadeTo(FadeMode mode, double time);
  void jumpTo(FadeMode mode);

  void updateFade(double time);

private:
  enum class State
  {
    FadingIn,
    FadingOut,
    Visible,
    Hidden
  };

  osg::ref_ptr<osgHelper::Animation<float>> m_animationIn;
  osg::ref_ptr<osgHelper::Animation<float>> m_animationOut;

  State m_state;

};

}
