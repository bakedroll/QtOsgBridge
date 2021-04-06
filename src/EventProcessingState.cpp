#include <QtOsgBridge/EventProcessingState.h>

#include <QMouseEvent>

namespace QtOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector)
  : AbstractEventState(injector)
{

}

bool EventProcessingState::isLoadingState() const
{
  return false;
}

bool EventProcessingState::eventFilter(QObject* object, QEvent* event)
{
  switch (event->type())
  {
  case QEvent::Type::KeyPress:
  case QEvent::Type::KeyRelease:
  {
    const auto keyEvent = dynamic_cast<QKeyEvent*>(event);
    assert_return(keyEvent, false);

    return onKeyEvent(keyEvent);
  }
  case QEvent::Type::MouseButtonDblClick:
  case QEvent::Type::MouseButtonPress:
  case QEvent::Type::MouseButtonRelease:
  case QEvent::Type::MouseMove:
  case QEvent::Type::DragEnter:
  case QEvent::Type::DragLeave:
  case QEvent::Type::DragMove:
  case QEvent::Type::DragResponse:
  {
    const auto mouseEvent = dynamic_cast<QMouseEvent*>(event);
    assert_return(mouseEvent, false);

    return onMouseEvent(mouseEvent);
  }
  default:
    break;
  }

  return QObject::eventFilter(object, event);
}

bool EventProcessingState::onKeyEvent(QKeyEvent* event)
{
  return false;
}

bool EventProcessingState::onMouseEvent(QMouseEvent* event)
{
  return false;
}

}
