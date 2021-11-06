#include <QtOsgBridge/QtGameApplication.h>
#include <QtOsgBridge/MainWindow.h>
#include <QtOsgBridge/Helper.h>
#include <QtOsgBridge/ViewProvider.h>

#include <osgHelper/Observable.h>
#include <osgHelper/TextureFactory.h>
#include <osgHelper/ShaderFactory.h>
#include <osgHelper/ResourceManager.h>

#include <utilsLib/StdOutLoggingStrategy.h>
#include <utilsLib/FileLoggingStrategy.h>
#include <utilsLib/Utils.h>

#include <QDir>
#include <QMessageBox>
#include <QPointer>

namespace QtOsgBridge
{

struct QtGameApplication::Impl
{
  Impl()
    : mainWindow(nullptr)
    , simData({0.0, 0.0})
  {
  }

  QPointer<MainWindow>               mainWindow;
  AbstractEventState::SimulationData simData;
};

QtGameApplication::QtGameApplication(int& argc, char** argv)
  : QtUtilsApplication<osg::ref_ptr<osg::Referenced>>(argc, argv)
  , GameApplication()
  , m(new Impl())
{
  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(
    std::make_shared<utilsLib::StdOutLoggingStrategy>());
  utilsLib::ILoggingManager::getLogger()->addLoggingStrategy(
    std::make_shared<utilsLib::FileLoggingStrategy>("./Logs"));

  setlocale(LC_NUMERIC, "en_US");

  m->mainWindow = new MainWindow();
  m->mainWindow->show();
}

QtGameApplication::~QtGameApplication()
{
  UTILS_LOG_INFO("Application shutting down");

  if (m->mainWindow->isVisible())
  {
    m->mainWindow->close();
  }

  m->mainWindow->deleteLater();
}

bool QtGameApplication::notify(QObject* receiver, QEvent* event)
{
  if (safeExecute([&]() { MultithreadedApplication::notify(receiver, event); return 0; }))
  {
      return true;
  }

  return false;
}

int QtGameApplication::runGame()
{
  return safeExecute([this]()
  {
    if (m_states.size() != 1)
    {
      UTILS_LOG_FATAL("Unexpected number of states. runGame() should only be called once.");
      return -1;
    }

    m_updateCallback = new GameUpdateCallback(std::bind(&QtGameApplication::updateStates, this, std::placeholders::_1));

    auto view = m->mainWindow->getViewWidget()->getView();
    view->getRootGroup()->setUpdateCallback(m_updateCallback);

    UTILS_LOG_INFO("Starting mainloop");
    const auto ret = exec();

    // shutdown/free all pointers
    for (auto& state : m_states)
    {
      state.state->onExit();
    }

    m_states.clear();
    view->cleanUp();

    return ret;
  });
}

void QtGameApplication::prepareEventState(StateData& data)
{
  data.connections.push_back(connect(data.state.get(), &AbstractEventState::forwardNewEventStateRequest, this,
                                     &QtGameApplication::onNewEventStateRequest));
  data.connections.push_back(connect(data.state.get(), &AbstractEventState::forwardExitEventStateRequest, this,
                                     &QtGameApplication::onExitEventStateRequest));

  data.connections.push_back(connect(data.state.get(), &AbstractEventState::forwardResetTimeDeltaRequest, [this]()
  {
    m_updateCallback->resetTimeDelta();
  }));

  data.state->onInitialize(m->mainWindow, m->simData);
  m->mainWindow->getViewWidget()->installEventFilter(data.state.get());
}

void QtGameApplication::onException(const std::string& message)
{
  QMessageBox::critical(nullptr, tr("Fatal error"), tr("A critical exception occured: %1").arg(QString::fromLocal8Bit(message.c_str())));
  quit();
}

void QtGameApplication::registerEssentialComponents(osgHelper::ioc::InjectionContainer& container)
{
  container.registerSingletonInterfaceType<osgHelper::IShaderFactory, osgHelper::ShaderFactory>();
  container.registerSingletonInterfaceType<osgHelper::IResourceManager, osgHelper::ResourceManager>();
  container.registerSingletonInterfaceType<osgHelper::ITextureFactory, osgHelper::TextureFactory>();

  container.registerSingletonInterfaceType<QtOsgBridge::IViewProvider, QtOsgBridge::ViewProvider>();
}

void QtGameApplication::updateStates(const osgHelper::SimulationCallback::SimulationData& data)
{
  m->simData = data;

  if (m_states.empty() && m->mainWindow->isVisible())
  {
    UTILS_LOG_DEBUG("Empty state list. Closing mainwindow");
    m->mainWindow->close();
  }

  for (auto& state : m_states)
  {
    state.state->onUpdate(data);
  }
}

void QtGameApplication::pushAndPrepareState(const osg::ref_ptr<AbstractEventState>& state)
{
  StateData data;
  data.state = state;

  m_states.push_back(data);
  prepareEventState(data);
}

void QtGameApplication::exitState(const osg::ref_ptr<AbstractEventState>& state)
{
  for (auto it = m_states.begin(); it != m_states.end(); ++it)
  {
    if (it->state == state)
    {
      m->mainWindow->removeEventFilter(state.get());
      state->onExit();

      m_states.erase(it);
      return;
    }
  }

  UTILS_LOG_FATAL("Attempting to exit unknown state");
}

void QtGameApplication::prepareViewProvider()
{
  const auto view = m->mainWindow->getViewWidget()->getView();
  auto viewProvider = injector().inject<IViewProvider>();
  if (viewProvider.valid())
  {
    viewProvider->setView(view);
  }
}

void QtGameApplication::onNewEventStateRequest(const osg::ref_ptr<AbstractEventState>& current,
                                               AbstractEventState::NewEventStateMode   mode,
                                               const osg::ref_ptr<AbstractEventState>& newState)
{
  if (mode == AbstractEventState::NewEventStateMode::ExitCurrent)
  {
    exitState(current);
  }

  pushAndPrepareState(newState);
}

void QtGameApplication::onExitEventStateRequest(const osg::ref_ptr<AbstractEventState>& current,
                                                AbstractEventState::ExitEventStateMode  mode)
{
  if (mode == AbstractEventState::ExitEventStateMode::ExitCurrent)
  {
    exitState(current);
    return;
  }

  auto it = m_states.begin();
  while (it != m_states.end())
  {
    exitState(it->state);
    it = m_states.begin();
  }
}

}
