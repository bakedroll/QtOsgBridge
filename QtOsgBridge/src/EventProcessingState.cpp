#include <QtOsgBridge/EventProcessingState.h>

#include <QMouseEvent>

namespace QtOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector)
  : AbstractEventState(injector)
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
      const osg::Vec2f origin(static_cast<float>(mouseEvent->position().x()), static_cast<float>(mouseEvent->position().y()));
      m_dragStates[mouseEvent->button()] = DragState{false, origin, origin};

      break;
    }
    case QEvent::Type::MouseMove:
    {
      if (m_dragStates.empty())
      {
        break;
      }

      for (auto& state : m_dragStates)
      {
        if (!state.second.moved)
        {
          onDragBegin(state.first, state.second.origin);
          state.second.moved = true;
        }

        const osg::Vec2f pos(static_cast<float>(mouseEvent->position().x()), static_cast<float>(mouseEvent->position().y()));

        onDragMove(state.first, state.second.origin, pos, state.second.lastPos - pos);

        state.second.lastPos = pos;
      }

      break;
    }
    case QEvent::Type::MouseButtonRelease:
    {
      const auto button = mouseEvent->button();

      if (m_dragStates.count(button) > 0)
      {
        onDragEnd(button, m_dragStates[button].origin,
                  osg::Vec2f(static_cast<float>(mouseEvent->position().x()), static_cast<float>(mouseEvent->position().y())));

        m_dragStates.erase(m_dragStates.find(button));
      }

      break;
    }
    default:
      break;
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

}  // namespace QtOsgBridge
