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

    void onInitialize(QPointer<MainWindow> mainWindow) override;
    void onExit() override;

  protected:
    virtual void onInitializeLoading(QPointer<MainWindow> mainWindow);
    virtual void onExitLoading();
    virtual void onLoading() = 0;

  private:
    QThread              m_loadingThread;
    LoadingThreadContext m_threadContext;

  };

}