#include <QtOsgBridge/AbstractEventState.h>

namespace QtOsgBridge
{

AbstractEventState::AbstractEventState(osgHelper::ioc::Injector& injector)
  : QObject()
  , osg::Referenced()
  , m_injector(&injector)
{
}

void AbstractEventState::onInitialize(MainWindow* mainWindow)
{
}

void AbstractEventState::onExit()
{
}

void AbstractEventState::requestExitEventState(ExitEventStateMode mode)
{
  Q_EMIT forwardExitEventStateRequest(this, mode);
}

}
