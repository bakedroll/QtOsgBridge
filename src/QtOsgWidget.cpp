#include <QtOsgBridge/QtOsgWidget.h>

#include <QKeyEvent>

namespace QtOsgBridge
{
  unsigned int getOsgMouseButton(QMouseEvent* event)
  {
    auto button = 0U;

    switch(event->button())
    {
    case Qt::LeftButton:
      button = 1;
      break;

    case Qt::MiddleButton:
      button = 2;
      break;

    case Qt::RightButton:
      button = 3;
      break;

    default:
      break;
    }

    return button;
  }

  QtOsgWidget::QtOsgWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_viewer(new Viewer())
    , m_view(new osgViewer::View())
    , m_camera(new osg::Camera())
    , m_graphicsWindow(new osgViewer::GraphicsWindowEmbedded(x(), y(), width(), height()))
    , m_updateMode(UpdateMode::OnInputEvent)
  {
    const auto w = width();
    const auto h = height();
    
    const auto aspectRatio = static_cast<float>(w) / h;

    m_camera->setProjectionMatrixAsPerspective(30.0f, aspectRatio, 1.0f, 1000.0f);
    m_camera->setGraphicsContext(m_graphicsWindow);

    onResize(w, h);

    m_view->setCamera(m_camera);

    m_viewer->addView(m_view);
    m_viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
    m_viewer->realize();

    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);

    connect(&m_updateTimer, &QTimer::timeout, this, QOverload<>::of(&QtOsgWidget::update));

    m_updateTimer.setSingleShot(false);
    setTargetFps(60);

    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_OpaquePaintEvent, false);
    setAutoFillBackground(false);
  }

  void QtOsgWidget::setUpdateMode(UpdateMode mode)
  {
    m_updateMode = mode;

    switch (m_updateMode)
    {
      case UpdateMode::OnInputEvent:
        m_updateTimer.stop();
        break;
      case UpdateMode::OnTimerEvent:
        m_updateTimer.start();
        break;
      default:
        break;
    }
  }

  void QtOsgWidget::setTargetFps(int fps)
  {
    m_updateTimer.setInterval(1000 / fps);
  }

  osg::ref_ptr<Viewer>& QtOsgWidget::getViewer()
  {
    return m_viewer;
  }

  osg::ref_ptr<osgViewer::View>& QtOsgWidget::getView()
  {
    return m_view;
  }

  void QtOsgWidget::paintGL()
  {
    m_viewer->frame();
  }

  void QtOsgWidget::resizeGL(int width, int height)
  {
    m_graphicsWindow->getEventQueue()->windowResize(x(), y(), width, height);
    m_graphicsWindow->resized(x(), y(), width, height);

    onResize(width, height);
  }

  void QtOsgWidget::paintEvent(QPaintEvent* paintEvent)
  {
    makeCurrent();
    paintGL();
    doneCurrent();
  }

  void QtOsgWidget::keyPressEvent(QKeyEvent* event)
  {
    QOpenGLWidget::keyPressEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    m_graphicsWindow->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KeySymbol(*keyData));
  }

  void QtOsgWidget::keyReleaseEvent(QKeyEvent* event)
  {
    QOpenGLWidget::keyReleaseEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    m_graphicsWindow->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
  }

  void QtOsgWidget::mouseMoveEvent(QMouseEvent* event)
  {
    QOpenGLWidget::mouseMoveEvent(event);

    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow->getEventQueue()->mouseMotion(static_cast<float>(event->x() * pixelRatio),
                                                   static_cast<float>(event->y() * pixelRatio));
  }

  void QtOsgWidget::mousePressEvent(QMouseEvent* event)
  {
    QOpenGLWidget::mousePressEvent(event);

    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow->getEventQueue()->mouseButtonPress(static_cast<float>(event->x() * pixelRatio),
        static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
  }

  void QtOsgWidget::mouseReleaseEvent(QMouseEvent* event)
  {
    QOpenGLWidget::mouseReleaseEvent(event);

    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow->getEventQueue()->mouseButtonRelease(static_cast<float>(event->x() * pixelRatio),
        static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
  }

  void QtOsgWidget::wheelEvent(QWheelEvent* event)
  {
    QOpenGLWidget::wheelEvent(event);

    event->accept();

    const auto delta  = event->angleDelta();
    const auto motion = (delta.y() > 0) ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;

    m_graphicsWindow->getEventQueue()->mouseScroll(motion);
  }

  bool QtOsgWidget::event(QEvent* event)
  {
    const auto handled = QOpenGLWidget::event(event);

    if (m_updateMode != UpdateMode::OnInputEvent)
    {
      return handled;
    }

    switch(event->type())
    {
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
    case QEvent::MouseMove:
    case QEvent::Wheel:
        update();
        break;

    default:
        break;
    }

    return handled;
  }

  void QtOsgWidget::onResize(int width, int height) const
  {
    const auto pixelRatio = this->devicePixelRatio();

    m_camera->setViewport(0, 0, width * pixelRatio, height * pixelRatio);
  }
}
