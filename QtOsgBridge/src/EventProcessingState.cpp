#include <QtOsgBridge/EventProcessingState.h>

#include <QMouseEvent>

#include "QtOsgBridge/KeyboardMouseEventFilter.h"

namespace QtOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector)
  : AbstractEventState(injector)
  , KeyboardMouseEventFilterBase()
{
}

bool EventProcessingState::eventFilter(QObject* object, QEvent* event)
{
  if (filterKeyboardMouseEvents(object, event))
  {
    return true;
  }

  return AbstractEventState::eventFilter(object, event);
}

}
