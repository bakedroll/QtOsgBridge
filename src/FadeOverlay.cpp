#include <QtOsgBridge/FadeOverlay.h>

namespace QtOsgBridge
{

FadeOverlay::FadeOverlay(float duration, bool isFadedOutInitially)
  : VirtualOverlay(),
    m_animationIn(new osgHelper::Animation<float>(0.0f, duration, osgHelper::AnimationEase::CIRCLE_IN)),
    m_animationOut(new osgHelper::Animation<float>(0.0f, duration, osgHelper::AnimationEase::CIRCLE_OUT)),
    m_state(isFadedOutInitially ? State::Hidden : State::Visible)
{
  if (isFadedOutInitially)
  {
    setVirtual(true);
    setVirtualVisible(false);
  }
}

FadeOverlay::~FadeOverlay() = default;

void FadeOverlay::fadeTo(FadeMode mode, double time)
{
  const auto isFadingIn  = ((m_state == State::FadingIn) || (m_state == State::Visible));
  const auto isFadingOut = ((m_state == State::FadingOut) || (m_state == State::Hidden));

  if ((isFadingIn && (mode == FadeMode::In)) || (isFadingOut && (mode == FadeMode::Out)))
  {
    return;
  }

  setVirtual(true);
  setVirtualVisible(true);

  if (mode == FadeMode::Out)
  {
    const auto value = (m_state == State::FadingIn) ? m_animationIn->getValue(time) : 1.0f;
    m_animationOut->beginAnimation(value, 0.0f, time);

    m_state = State::FadingOut;
  }
  else
  {
    const auto value = (m_state == State::FadingOut) ? m_animationOut->getValue(time) : 0.0f;
    m_animationIn->beginAnimation(value, 1.0f, time);

    m_state = State::FadingIn;
  }
}

void FadeOverlay::jumpTo(FadeMode mode)
{
  m_state = (mode == FadeMode::Out) ? State::Hidden : State::Visible;
  setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, 1.0f));

  if (mode == FadeMode::In)
  {
    setVirtual(false);
    setVirtualVisible(false);
  }
  else
  {
    setVirtual(true);
    setVirtualVisible(false);
  }
}

void FadeOverlay::updateFade(double time)
{
  if ((m_state == State::Hidden) || (m_state == State::Visible))
  {
    return;
  }

  auto alpha = 1.0f;

  if (m_state == State::FadingOut)
  {
    alpha = m_animationOut->getValue(time);
    if (alpha <= 0.0f)
    {
      setVirtualVisible(false);

      m_state = State::Hidden;
    }
  }
  else if (m_state == State::FadingIn)
  {
    alpha = m_animationIn->getValue(time);
    if (alpha >= 1.0f)
    {
      setVirtual(false);

      m_state = State::Visible;
    }
  }

  setColor(osg::Vec4f(1.0f, 1.0f, 1.0f, alpha));
}

}
