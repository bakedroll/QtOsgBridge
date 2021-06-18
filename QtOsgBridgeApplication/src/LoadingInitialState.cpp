#include "LoadingInitialState.h"
#include "InitialState.h"

#include <QtOsgBridge/Helper.h>

#include <QPointer>
#include <QVBoxLayout>
#include <QLabel>

LoadingInitialState::LoadingInitialState(osgHelper::ioc::Injector& injector)
  : LoadingState(injector)
{
}

void LoadingInitialState::onInitializeLoading(QPointer<QtOsgBridge::MainWindow> mainWindow, const SimulationData& data)
{
  m_mainWindow = mainWindow;

  QVBoxLayout* layout = new QVBoxLayout();
  layout->addWidget(new QLabel("Loading..."));
  layout->addStretch(1);

  m_overlay = new QWidget();
  m_overlay->setLayout(layout);
  m_overlay->setGeometry(0, 0, 300, 300);
  m_overlay->setStyleSheet("color: #fff; font-size: 36pt;");

  m_mainWindow->getViewWidget()->addOverlayWidget(m_overlay);
}

void LoadingInitialState::onExitLoading()
{
  QtOsgBridge::Helper::deleteWidget(m_overlay);
}

void LoadingInitialState::onLoading()
{
}

void LoadingInitialState::onRequestNewStates()
{
  requestNewEventState<InitialState>(NewEventStateMode::ExitCurrent);
}
