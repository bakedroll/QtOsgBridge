#include <QtOsgBridge/QtGameApplication.h>

#include <QtOsgBridge/EventProcessingState.h>
#include <QtOsgBridge/ViewProvider.h>

#include <QMessageBox>

namespace QtOsgBridge
{

QtGameApplication::QtGameApplication(int& argc, char** argv) :
  MultithreadedApplication<QApplication>(argc, argv),
  GameStatesApplication()
{
  m_mainWindow = new MainWindow();
  m_mainWindow->show();
}

QtGameApplication::~QtGameApplication()
{
  UTILS_LOG_INFO("Application shutting down");

  if (m_mainWindow->isVisible())
  {
    m_mainWindow->close();
  }

  m_mainWindow->deleteLater();
}

void QtGameApplication::onInitialize(const osg::ref_ptr<libQtGame::GameUpdateCallback>& updateCallback)
{
  const auto view = m_mainWindow->getViewWidget()->getView();
  view->getRootGroup()->setUpdateCallback(updateCallback);
}

void QtGameApplication::onPrepareGameState(
  const osg::ref_ptr<libQtGame::AbstractGameState>& state,
  const libQtGame::AbstractGameState::SimulationData& simData)
{
  auto* eventState = dynamic_cast<EventProcessingState*>(state.get());
  if (eventState)
  {
    eventState->onInitialize(m_mainWindow, simData);
    m_mainWindow->getViewWidget()->installEventFilter(eventState->eventHandler());
  }

  m_mainWindow->getViewWidget()->installEventFilter(state.get());
}

void QtGameApplication::onExitGameState(const osg::ref_ptr<libQtGame::AbstractGameState>& state)
{
  auto* eventState = dynamic_cast<EventProcessingState*>(state.get());
  if (eventState)
  {
    m_mainWindow->getViewWidget()->removeEventFilter(eventState->eventHandler());
  }

  m_mainWindow->removeEventFilter(state.get());
}

void QtGameApplication::onEmptyStateList()
{
  if (m_mainWindow->isVisible())
  {
    UTILS_LOG_DEBUG("Empty state list. Closing mainwindow");
    m_mainWindow->close();
  }
}

void QtGameApplication::onShutdown()
{
  m_mainWindow->getViewWidget()->getView()->cleanUp();
}

void QtGameApplication::registerEssentialComponents(osgHelper::ioc::InjectionContainer& container)
{
  GameStatesApplication::registerEssentialComponents(container);

  container.registerSingletonInterfaceType<QtOsgBridge::IViewProvider, QtOsgBridge::ViewProvider>();
}

bool QtGameApplication::notify(QObject* receiver, QEvent* event)
{
  if (safeExecute([&]() { QtUtilsLib::MultithreadedApplication<QApplication>::notify(receiver, event); return 0; }))
  {
    return true;
  }

  return false;
}

int QtGameApplication::execApp()
{
  return exec();
}

void QtGameApplication::quitApp()
{
  quit();
}

void QtGameApplication::onException(const std::string& message)
{
  QMessageBox::critical(nullptr, tr("Fatal error"), tr("A critical exception occured: %1").arg(QString::fromLocal8Bit(message.c_str())));
}

void QtGameApplication::prepareViewProvider()
{
  const auto view = m_mainWindow->getViewWidget()->getView();
  auto viewProvider = injector().inject<IViewProvider>();
  if (viewProvider.valid())
  {
    viewProvider->setView(view);
  }
}

}
