#include <QtOsgBridge/AbstractEventState.h>

#include <QtUtilsLib/MultithreadedApplication.h>

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

void AbstractEventState::onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data)
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
  }

  return QObject::eventFilter(object, event);
}

void AbstractEventState::requestExitEventState(ExitEventStateMode mode)
{
  QtUtilsLib::MultithreadedApplication::executeInUiAsync([this, mode]()
  {
    Q_EMIT forwardExitEventStateRequest(this, mode);
  });
}

void AbstractEventState::requestResetTimeDelta()
{
  QtUtilsLib::MultithreadedApplication::executeInUiAsync([this]()
  {
    Q_EMIT forwardResetTimeDeltaRequest();
  });
}

void AbstractEventState::onResizeEvent(QResizeEvent* event)
{
}

}
