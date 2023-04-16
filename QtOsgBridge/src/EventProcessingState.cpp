#include <QtOsgBridge/EventProcessingState.h>

#include <QMouseEvent>

namespace QtOsgBridge
{

EventProcessingState::EventProcessingState(osgHelper::ioc::Injector& injector)
  : AbstractGameState(injector)
  , m_eventHandler(new libQtGame::KeyboardMouseEventFilter(this))
{
  connect(m_eventHandler, &libQtGame::KeyboardMouseEventFilter::triggerKeyEvent,
    this, &EventProcessingState::forwardKeyEvent, Qt::DirectConnection);
  connect(m_eventHandler, &libQtGame::KeyboardMouseEventFilter::triggerMouseEvent,
    this, &EventProcessingState::forwardMouseEvent, Qt::DirectConnection);
  connect(m_eventHandler, &libQtGame::KeyboardMouseEventFilter::triggerWheelEvent,
    this, &EventProcessingState::forwardWheelEvent, Qt::DirectConnection);
  connect(m_eventHandler, &libQtGame::KeyboardMouseEventFilter::triggerDragBegin,
    this, &EventProcessingState::forwardDragBegin, Qt::DirectConnection);
  connect(m_eventHandler, &libQtGame::KeyboardMouseEventFilter::triggerDragMove,
    this, &EventProcessingState::forwardDragMove, Qt::DirectConnection);
  connect(m_eventHandler, &libQtGame::KeyboardMouseEventFilter::triggerDragEnd,
    this, &EventProcessingState::forwardDragEnd, Qt::DirectConnection);
}

void EventProcessingState::onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data)
{
}

bool EventProcessingState::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::Type::Resize)
  {
    const auto resizeEvent = dynamic_cast<QResizeEvent*>(event);
    assert_return(resizeEvent, false);

    onResizeEvent(resizeEvent);
  }

  return AbstractGameState::eventFilter(object, event);
}

QPointer<libQtGame::KeyboardMouseEventFilter> EventProcessingState::eventHandler() const
{
  return m_eventHandler;
}

void EventProcessingState::onResizeEvent(QResizeEvent* event)
{
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

void EventProcessingState::onDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
}

void EventProcessingState::onDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
}

void EventProcessingState::forwardKeyEvent(QKeyEvent* event, bool& accepted)
{
  accepted = onKeyEvent(event);
}

void EventProcessingState::forwardMouseEvent(QMouseEvent* event, bool& accepted)
{
  accepted = onMouseEvent(event);
}

void EventProcessingState::forwardWheelEvent(QWheelEvent* event, bool& accepted)
{
  accepted = onWheelEvent(event);
}

void EventProcessingState::forwardDragBegin(Qt::MouseButton button, const osg::Vec2f& origin)
{
  onDragBegin(button, origin);
}

void EventProcessingState::forwardDragMove(
  Qt::MouseButton button, const osg::Vec2f& origin,
  const osg::Vec2f& position, const osg::Vec2f& change)
{
  onDragMove(button, origin, position, change);
}

void EventProcessingState::forwardDragEnd(
  Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position)
{
  onDragEnd(button, origin, position);
}

}
