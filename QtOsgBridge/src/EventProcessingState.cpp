#include <QtOsgBridge/EventProcessingState.h>

#include <QMouseEvent>

#include "QtOsgBridge/KeyboardMouseEventFilter.h"

namespace QtOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector)
  : AbstractGameState(injector)
  , KeyboardMouseEventFilterBase()
{
}

void EventProcessingState::onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data)
{
}

bool EventProcessingState::eventFilter(QObject* object, QEvent* event)
{
  if (filterKeyboardMouseEvents(object, event))
  {
    return true;
  }

  if (event->type() == QEvent::Type::Resize)
  {
    const auto resizeEvent = dynamic_cast<QResizeEvent*>(event);
    assert_return(resizeEvent, false);

    onResizeEvent(resizeEvent);
  }

  return AbstractGameState::eventFilter(object, event);
}

void EventProcessingState::onResizeEvent(QResizeEvent* event)
{
}

}
