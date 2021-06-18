#pragma once

#include <QtOsgBridge/AbstractEventState.h>
#include <QtOsgBridge/LoadingThreadContext.h>

#include <QThread>

namespace QtOsgBridge
{
  class LoadingState : public AbstractEventState
  {
  public:
    explicit LoadingState(osgHelper::ioc::Injector& injector);

    void onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data) override;
    void onExit() override;

  protected:
    virtual void onInitializeLoading(QPointer<MainWindow> mainWindow, const SimulationData& data);
    virtual void onExitLoading();
    virtual void onLoading() = 0;

  protected Q_SLOTS:
    virtual void onRequestNewStates() = 0;

  private:
    QThread              m_loadingThread;
    LoadingThreadContext m_threadContext;

  };

}