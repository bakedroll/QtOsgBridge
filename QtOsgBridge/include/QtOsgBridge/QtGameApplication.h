#pragma once

#include <QApplication>

#include <QtUtilsLib/MultithreadedApplication.h>

#include <libQtGame/GameStatesApplication.h>

#include <QtOsgBridge/MainWindow.h>

namespace QtOsgBridge
{

class QtGameApplication : public QtUtilsLib::MultithreadedApplication<QApplication>,
                          public libQtGame::GameStatesApplication
{
  Q_OBJECT

public:
  QtGameApplication(int& argc, char** argv);
  ~QtGameApplication();

  template <typename TState>
  int runGame()
  {
    setupIOC();

    prepareViewProvider();
    if (!injectPushAndPrepareState<TState>())
    {
      return -1;
    }

    return GameStatesApplication::runGame();
  }

  bool notify(QObject *receiver, QEvent *event) override;

  int execApp() override;
  void quitApp() override;

protected:
  void onException(const std::string& message) override;

  void onInitialize(const osg::ref_ptr<libQtGame::GameUpdateCallback>& updateCallback) override;
  void onPrepareGameState(
    const osg::ref_ptr<libQtGame::AbstractGameState>& state,
    const libQtGame::AbstractGameState::SimulationData& simData) override;
  void onExitGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state) override;
  void onEmptyStateList() override;
  void onShutdown() override;

  void registerEssentialComponents(osgHelper::ioc::InjectionContainer& container) override;

private:
  QPointer<MainWindow> m_mainWindow;

  void prepareViewProvider();

};

}
