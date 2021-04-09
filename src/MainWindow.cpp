#include "QtOsgBridge/MainWindow.h"

#include <QFrame>
#include <QApplication>
#include <QVBoxLayout>
#include <QScreen>

#include <QMouseEvent>

namespace QtOsgBridge
{

struct MainWindow::Impl
{
  QtOsgWidget* osgWidget;
};

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , m(new Impl())
{
  m->osgWidget = new QtOsgWidget();
  m->osgWidget->setUpdateMode(QtOsgWidget::UpdateMode::OnTimerEvent);

  for (const auto& screen : QGuiApplication::screens())
  {
    const auto geo = screen->geometry();

    const auto winWidth  = 1280;
    const auto winHeight = 768;

    setGeometry(QRect(geo.width() / 2 - winWidth / 2, geo.height() / 2 - winHeight / 2, winWidth, winHeight));
    //setMinimumSize(winWidth, winHeight);

    break;
  }

  setCentralWidget(m->osgWidget);
}

MainWindow::~MainWindow() = default;

QPointer<QtOsgWidget> MainWindow::getViewWidget() const
{
  return m->osgWidget;
}

void MainWindow::shutdown()
{
  m->osgWidget->setParent(nullptr);
  close();
}

}
