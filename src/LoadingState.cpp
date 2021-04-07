#include <QtOsgBridge/LoadingState.h>

#include <QPointer>

namespace QtOsgBridge
{

LoadingState::LoadingState(osgHelper::ioc::Injector& injector)
  : AbstractEventState(injector)
  , m_threadContext(std::bind(&LoadingState::onLoading, this))
{
}

void LoadingState::onInitialize(QPointer<MainWindow> mainWindow)
{
  onInitializeLoading(mainWindow);

  m_threadContext.moveToThread(&m_loadingThread);
  m_loadingThread.start();

  QMetaObject::invokeMethod(&m_threadContext, &LoadingThreadContext::doWork, Qt::QueuedConnection);
}

void LoadingState::onExit()
{
  m_loadingThread.quit();
  m_loadingThread.wait();

  onExitLoading();

  requestResetTimeDelta();
}

void LoadingState::onInitializeLoading(QPointer<MainWindow> mainWindow)
{
}

void LoadingState::onExitLoading()
{
}

}
