#include <QtOsgBridge/QtGameApplication.h>
#include <QtOsgBridge/MainWindow.h>

#include <osgHelper/Observable.h>
#include <osgHelper/LogManager.h>

#include <QDir>
#include <QDateTime>
#include <QTextStream>
#include <QMessageBox>

#include <cassert>

namespace QtOsgBridge
{

class FileLogger : public osgHelper::Logger
{
public:
  FileLogger(const QString& directory)
  {
    const QDir dir(directory);
    if (!dir.exists())
    {
      dir.mkpath(".");
    }

    const auto filename =
            dir.filePath(QString("%1.txt").arg(QDateTime::currentDateTime().toString("yy-MM-dd_HH.mm.ss")));

    m_file.setFileName(filename);
    m_file.open(QIODevice::WriteOnly);

    m_textStream.setDevice(&m_file);
  }

  ~FileLogger()
  {
    if (m_file.isOpen())
      m_file.close();
  }

  void log(const std::string& text) override
  {
    if (m_file.isOpen())
    {
      m_textStream << QString::fromStdString(text) << "\n";
      m_textStream.flush();
    }
  }

private:
  QFile m_file;
  QTextStream m_textStream;
};

struct QtGameApplication::Impl
{
  Impl()
    : mainWindow(nullptr)
  {
  }

  QtOsgBridge::MainWindow* mainWindow;
};

QtGameApplication::QtGameApplication(int& argc, char** argv)
  : Multithreading(argc, argv)
  , GameApplication()
  , m(new Impl())
  , m_injector(nullptr)
{
  osgHelper::LogManager::getInstance()->addLogger(new osgHelper::StdOutLogger());
  osgHelper::LogManager::getInstance()->addLogger(new FileLogger("./Logs"));

  setlocale(LC_NUMERIC, "en_US");

  m->mainWindow = new MainWindow();
  m->mainWindow->show();
}

QtGameApplication::~QtGameApplication() = default;

/*void QtGameApplication::action(osg::Object* object, osg::Object* data, double simTime, double timeDiff)
{

}*/

bool QtGameApplication::notify(QObject* receiver, QEvent* event)
{
  if (safeExecute([&]() { Multithreading::notify(receiver, event); return 0; }))
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
      OSGG_LOG_FATAL("Unexpected number of states. runGame() should only be called once.");
      return -1;
    }

    auto view = m->mainWindow->getViewWidget()->getView();
    // view->getRootGroup()->setUpdateCallback(this);

    OSGG_LOG_INFO("Starting mainloop");
    const auto ret = exec();

    deinitialize();

    // shutdown/free all pointers
    m_states.clear();
    view->cleanUp();

    container().clear();

    osgHelper::LogManager::clearInstance();

    return ret;
  });
}

void QtGameApplication::prepareEventState(StateData& data)
{
  data.state->onInitialize(m->mainWindow);
  m->mainWindow->getViewWidget()->installEventFilter(data.state.get());

  data.connections.push_back(connect(data.state.get(), &AbstractEventState::forwardNewEventStateRequest, this,
                                     &QtGameApplication::onNewEventStateRequest));
  data.connections.push_back(connect(data.state.get(), &AbstractEventState::forwardExitEventStateRequest, this,
                                     &QtGameApplication::onExitEventStateRequest));
}

void QtGameApplication::deinitialize()
{
  OSGG_LOG_INFO("Application shutting down");

  m->mainWindow->close();
  delete m->mainWindow;
}

void QtGameApplication::onException(const std::string& message)
{
  QMessageBox::critical(nullptr, tr("Fatal error"), tr("A critical exception occured: %1").arg(QString::fromLocal8Bit(message.c_str())));
  quit();
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

  OSGG_LOG_FATAL("Attempting to exit unknown state");
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

  for (const auto& data : m_states)
  {
    exitState(data.state);
  }
}

}
