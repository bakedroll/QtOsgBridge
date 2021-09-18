#include <QtOsgBridge/EventProcessingState.h>

#include <QMouseEvent>

namespace QtOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector)
  : AbstractEventState(injector)
  , m_isMouseCaptured(false)
{

}

bool EventProcessingState::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease)
  {
    const auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
    assert_return(mouseEvent, false);

    m_isMouseDown[static_cast<Qt::MouseButton>(mouseEvent->button())] = (event->type() == QEvent::Type::MouseButtonPress);
  }

  switch (event->type())
  {
  case QEvent::Type::KeyPress:
  case QEvent::Type::KeyRelease:
  {
    const auto keyEvent = dynamic_cast<QKeyEvent*>(event);
    assert_return(keyEvent, false);

    m_isKeyDown[static_cast<Qt::Key>(keyEvent->key())] = (event->type() == QEvent::Type::KeyPress);

    return onKeyEvent(keyEvent);
  }
  case QEvent::Type::MouseButtonPress:
  case QEvent::Type::MouseButtonRelease:
  case QEvent::Type::MouseButtonDblClick:
  case QEvent::Type::MouseMove:
  {
    const auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
    assert_return(mouseEvent, false);

    switch (event->type())
    {
    case QEvent::Type::MouseButtonPress:
    {
      if (!m_mouseDragData)
      {
        const osg::Vec2f origin(static_cast<float>(mouseEvent->position().x()), static_cast<float>(mouseEvent->position().y()));
        m_mouseDragData = MouseDragData{ mouseEvent->button(), false, origin, origin };
      }

      break;
    }
    case QEvent::Type::MouseMove:
    {
      if (!m_mouseDragData)
      {
        break;
      }

      if (!m_mouseDragData->moved)
      {
        onDragBegin(m_mouseDragData->button, m_mouseDragData->origin);
        m_mouseDragData->moved = true;
      }

      const osg::Vec2f pos(static_cast<float>(mouseEvent->position().x()), static_cast<float>(mouseEvent->position().y()));

      if (m_isMouseCaptured)
      {
        const auto delta = m_capturedMousePos - mouseEvent->globalPosition();
        onDragMove(m_mouseDragData->button, m_mouseDragData->origin,
                   pos, osg::Vec2f(static_cast<int>(delta.x()), static_cast<int>(delta.y())));
      }
      else
      {
        onDragMove(m_mouseDragData->button, m_mouseDragData->origin,
                   pos, m_mouseDragData->lastPos - pos);
      }

      m_mouseDragData->lastPos = pos;

      break;
    }
    case QEvent::Type::MouseButtonRelease:
    {
      if (m_mouseDragData && (m_mouseDragData->button == mouseEvent->button()))
      {
        onDragEnd(m_mouseDragData->button, m_mouseDragData->origin,
                  osg::Vec2f(static_cast<float>(mouseEvent->position().x()), static_cast<float>(mouseEvent->position().y())));

        m_mouseDragData.reset();
      }

      break;
    }
    default:
      break;
    }

    if (event->type() == QEvent::Type::MouseMove && m_isMouseCaptured)
    {
      QCursor::setPos(m_capturedMousePos);
    }

    return onMouseEvent(mouseEvent);
  }
  case QEvent::Type::Wheel:
  {
    const auto wheelEvent = dynamic_cast<QWheelEvent*>(event);
    assert_return(wheelEvent, false);

    return onWheelEvent(wheelEvent);
  }
  default:
    break;
  }

  return AbstractEventState::eventFilter(object, event);
}

bool EventProcessingState::onKeyEvent(QKeyEvent* event)
{
  return false;
}

bool EventProcessingState::onMouseEvent(QMouseEvent* event)
{
  return false;
}

bool EventProcessingState::onWheelEvent(QWheelEvent* event)
{
  return false;
}

void EventProcessingState::onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
}

void EventProcessingState::onDragMove(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position,
                                      const osg::Vec2f& change)
{
}

void EventProcessingState::onDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
}

bool EventProcessingState::isKeyDown(Qt::Key key) const
{
  if (m_isKeyDown.count(key) == 0)
  {
    return false;
  }

  return m_isKeyDown.find(key)->second;
}

bool EventProcessingState::isMouseButtonDown(Qt::MouseButton button) const
{
  if (m_isMouseDown.count(button) == 0)
  {
    return false;
  }

  return m_isMouseDown.find(button)->second;
}

bool EventProcessingState::isMouseDragging(const std::optional<Qt::MouseButton>& button) const
{
  return m_mouseDragData && m_mouseDragData->moved && (!button || (*button == m_mouseDragData->button));
}

void EventProcessingState::setCaptureMouse(bool on)
{
  if (m_isMouseCaptured == on)
  {
    return;
  }

  m_isMouseCaptured  = on;
  m_capturedMousePos = QCursor::pos();
}

}  // namespace QtOsgBridge
