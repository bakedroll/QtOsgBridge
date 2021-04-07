#include <QtOsgBridge/AbstractEventState.h>

#include <QPointer>

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

}
