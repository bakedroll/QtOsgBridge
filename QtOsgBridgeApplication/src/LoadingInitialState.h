#pragma once

#include <QtOsgBridge/LoadingState.h>

#include <QPointer>

class LoadingInitialState : public QtOsgBridge::LoadingState
{
public:
  explicit LoadingInitialState(osgHelper::ioc::Injector& injector);

protected:
  void onInitializeLoading(QPointer<QtOsgBridge::MainWindow> mainWindow, const SimulationData& data) override;
  void onExitLoading() override;
  void onLoading()  override;
  void onRequestNewStates() override;

private:
  QPointer<QtOsgBridge::MainWindow> m_mainWindow;
  QPointer<QWidget>                 m_overlay;

};