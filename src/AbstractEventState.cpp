#include <QtOsgBridge/AbstractEventState.h>

#include <QPointer>
#include <QResizeEvent>

namespace QtOsgBridge
{

AbstractEventState::AbstractEventState(osgHelper::ioc::Injector& injector)
  : QObject()
  , osg::Referenced()
  , m_injector(&injector)
{
}

void AbstractEventState::onInitialize(QPointer<MainWindow> mainWindow)
{
}

void AbstractEventState::onUpdate(const SimulationData& data)
{
}

void AbstractEventState::onExit()
{
}

bool AbstractEventState::eventFilter(QObject* object, QEvent* event)
{
  if (event->type() == QEvent::Type::Resize)
  {
    const auto resizeEvent = dynamic_cast<QResizeEvent*>(event);
    assert_return(resizeEvent, false);

    onResizeEvent(resizeEvent);
    return true;
  }

  return false;
}

void AbstractEventState::requestExitEventState(ExitEventStateMode mode)
{
  Multithreading::executeInUiAsync([this, mode]()
  {
    Q_EMIT forwardExitEventStateRequest(this, mode);
  });
}

void AbstractEventState::requestResetTimeDelta()
{
  Multithreading::executeInUiAsync([this]()
  {
    Q_EMIT forwardResetTimeDeltaRequest();
  });
}

void AbstractEventState::onResizeEvent(QResizeEvent* event)
{
}

}