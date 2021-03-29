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
    QDir dir(directory);
    if (!dir.exists())
      dir.mkpath(".");

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
    , oFullscreenEnabled(new osgHelper::Observable<bool>(false))
  {
  }

  QtOsgBridge::MainWindow* mainWindow;

  osgHelper::Observable<bool>::Ptr oFullscreenEnabled;

  osgHelper::Observer<void>::Ptr endGameObserver;
  osgHelper::Observer<bool>::Ptr fullscreenEnabledObserver;
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

  m->endGameObserver = onEndGameSignal().connect([this]()
  {
    m->mainWindow->shutdown();
  });

  m->fullscreenEnabledObserver = m->oFullscreenEnabled->connect([this](bool enabled)
  {
    if (m->mainWindow->isFullScreen() == enabled)
      return;

    if (enabled)
      m->mainWindow->showFullScreen();
    else
      m->mainWindow->showNormal();
  });
}

QtGameApplication::~QtGameApplication() = default;

void QtGameApplication::action(osg::Object* object, osg::Object* data, double simTime, double timeDiff)
{

}

bool QtGameApplication::notify(QObject* receiver, QEvent* event)
{
  if (safeExecute([&]() { Multithreading::notify(receiver, event); return 0; }))
  {
      return true;
  }

  return false;
}

int QtGameApplication::runGame(const osg::ref_ptr<AbstractEventState>& initialState)
{
  return safeExecute([&]()
  {
    const auto view = m->mainWindow->getViewWidget()->getView();

    view->getRootGroup()->setUpdateCallback(this);

    prepareEventState(initialState);

    OSGG_LOG_INFO("Starting mainloop");
    const auto ret = mainloop();

    deinitialize();

    // shutdown/free all pointers
    view->setSceneData(nullptr);

    container().clear();

    osgHelper::LogManager::clearInstance();

    return ret;
  });
}

void QtGameApplication::prepareEventState(const osg::ref_ptr<AbstractEventState>& state)
{
  state->initialize(m->mainWindow);
}

void QtGameApplication::deinitialize()
{
  OSGG_LOG_INFO("Application shutting down");

  m->mainWindow->close();
  delete m->mainWindow;
}

int QtGameApplication::mainloop()
{
  return exec();
}

void QtGameApplication::onException(const std::string& message)
{
  QMessageBox::critical(nullptr, tr("Fatal error"), tr("A critical exception occured: %1").arg(QString::fromLocal8Bit(message.c_str())));
  quit();
}

}
