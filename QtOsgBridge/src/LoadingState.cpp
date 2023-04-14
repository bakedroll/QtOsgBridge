#include <QtOsgBridge/LoadingState.h>

#include <QPointer>

namespace QtOsgBridge
{

LoadingState::LoadingState(osgHelper::ioc::Injector& injector)
  : EventProcessingState(injector)
  , m_threadContext(std::bind(&LoadingState::onLoading, this))
{
  connect(&m_threadContext, &LoadingThreadContext::workDone, this, &LoadingState::onRequestNewStates);
}

void LoadingState::onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data)
{
  onInitializeLoading(mainWindow, data);

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

void LoadingState::onInitializeLoading(QPointer<MainWindow> mainWindow, const SimulationData& data)
{
}

void LoadingState::onExitLoading()
{
}

}
