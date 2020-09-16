#include <QtOsgBridge/QtOsgWidget.h>

#include <osgHelper/Macros.h>

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
    : GLWidgetBase(parent)
    , m_graphicsWindow(new osgViewer::GraphicsWindowEmbedded(x(), y(), width(), height()))
    , m_updateMode(UpdateMode::OnInputEvent)
  {
    const auto w          = width();
    const auto h          = height();
    const auto pixelRatio = devicePixelRatio();

    // const auto aspectRatio = static_cast<float>(w) / h;

    const auto numLayers = static_cast<int>(ViewType::_Count);
    m_renderLayers.resize(numLayers);
    for (auto i=0; i<numLayers; i++)
    {
      RenderLayer layer;

      layer.view   = new osgHelper::View();
      layer.camera = new osgHelper::Camera(*layer.view->getCamera());

      layer.camera->setGraphicsContext(m_graphicsWindow);
      layer.view->setCamera(layer.camera);
      layer.view->updateViewport(0, 0, w, h, pixelRatio);

      layer.viewer = new osgViewer::CompositeViewer();
      layer.viewer->addView(layer.view);
      layer.viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
      layer.viewer->realize();

      m_renderLayers[i] = layer;
    }

    auto screenCamera = m_renderLayers[static_cast<int>(ViewType::Screen)].camera;

    screenCamera->setClearMask(GL_DEPTH_BUFFER_BIT);
    screenCamera->setProjectionMode(osgHelper::Camera::ProjectionMode::Ortho2D);

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

  osg::ref_ptr<osgHelper::View> QtOsgWidget::getView(ViewType type) const
  {
    const auto index = static_cast<int>(type);
    assert_return(index < static_cast<int>(ViewType::_Count), osgHelper::View::Ptr());

    return m_renderLayers[index].view;
  }

  osg::ref_ptr<osgHelper::Camera> QtOsgWidget::getCamera(ViewType type) const
  {
    const auto index = static_cast<int>(type);
    assert_return(index < static_cast<int>(ViewType::_Count), osgHelper::Camera::Ptr());

    return m_renderLayers[index].camera;
  }

  void QtOsgWidget::paintGL()
  {
    const auto numViewers = static_cast<int>(ViewType::_Count);
    for (auto i = 0; i < numViewers; i++)
    {
        m_renderLayers[i].viewer->frame();
    }
  }

  void QtOsgWidget::resizeGL(int width, int height)
  {
    const auto numViewers = static_cast<int>(ViewType::_Count);
    for (auto i=0; i<numViewers; i++)
    {
      m_renderLayers[i].view->updateResolution(osg::Vec2f(width, height), devicePixelRatio());
      m_renderLayers[i].camera->updateResolution(osg::Vec2i(width, height));
    }

    m_graphicsWindow->getEventQueue()->windowResize(x(), y(), width, height);
    m_graphicsWindow->resized(x(), y(), width, height);
  }

  void QtOsgWidget::paintEvent(QPaintEvent* paintEvent)
  {
    makeCurrent();
    paintGL();
    doneCurrent();
  }

  void QtOsgWidget::keyPressEvent(QKeyEvent* event)
  {
      GLWidgetBase::keyPressEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    m_graphicsWindow->getEventQueue()->keyPress(osgGA::GUIEventAdapter::KeySymbol(*keyData));
  }

  void QtOsgWidget::keyReleaseEvent(QKeyEvent* event)
  {
      GLWidgetBase::keyReleaseEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    m_graphicsWindow->getEventQueue()->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
  }

  void QtOsgWidget::mouseMoveEvent(QMouseEvent* event)
  {
      GLWidgetBase::mouseMoveEvent(event);

    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow->getEventQueue()->mouseMotion(static_cast<float>(event->x() * pixelRatio),
                                                   static_cast<float>(event->y() * pixelRatio));
  }

  void QtOsgWidget::mousePressEvent(QMouseEvent* event)
  {
      GLWidgetBase::mousePressEvent(event);

    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow->getEventQueue()->mouseButtonPress(static_cast<float>(event->x() * pixelRatio),
        static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
  }

  void QtOsgWidget::mouseReleaseEvent(QMouseEvent* event)
  {
      GLWidgetBase::mouseReleaseEvent(event);

    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow->getEventQueue()->mouseButtonRelease(static_cast<float>(event->x() * pixelRatio),
        static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
  }

  void QtOsgWidget::wheelEvent(QWheelEvent* event)
  {
      GLWidgetBase::wheelEvent(event);

    event->accept();

    const auto delta  = event->angleDelta();
    const auto motion = (delta.y() > 0) ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;

    m_graphicsWindow->getEventQueue()->mouseScroll(motion);
  }

  bool QtOsgWidget::event(QEvent* event)
  {
    const auto handled = GLWidgetBase::event(event);

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
}
