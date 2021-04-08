#include <QtOsgBridge/QtOsgWidget.h>
#include <QtOsgBridge/OverlayCompositor.h>

#include <osgViewer/Viewer>

#include <QKeyEvent>
#include <QPainter>

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
    , m_updateMode(UpdateMode::OnInputEvent)
    , m_isFirstFrame(true)
    , m_overlayCompositor(new OverlayCompositor(this))
  {
    const auto w          = width();
    const auto h          = height();
    const auto pixelRatio = devicePixelRatio();

    m_graphicsWindow = new osgViewer::GraphicsWindowEmbedded(0, 0, w * pixelRatio, h * pixelRatio);

    m_view = new osgHelper::View();
    m_view->getCamera(osgHelper::View::CameraType::Scene)->setGraphicsContext(m_graphicsWindow);
    m_view->getCamera(osgHelper::View::CameraType::Screen)->setGraphicsContext(m_graphicsWindow);
    m_view->updateCameraViewports(0, 0, w, h, pixelRatio);

    m_graphicsWindow->getEventQueue()->syncWindowRectangleWithGraphicsContext();

    m_viewer = new osgViewer::CompositeViewer();
    m_viewer->addView(m_view);
    m_viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
    m_viewer->setReleaseContextAtEndOfFrameHint(false);

    setupVirtualOverlayNodes();

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

  QtOsgWidget::~QtOsgWidget()
  {
    m_overlayCompositor->clear();
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

  osg::ref_ptr<osgHelper::View> QtOsgWidget::getView() const
  {
    return m_view;
  }

  osg::ref_ptr<OverlayCompositor> QtOsgWidget::getOverlayCompositor() const
  {
    return m_overlayCompositor;
  }

  void QtOsgWidget::initializeGL()
  {
    initializeOpenGLFunctions();
  }

  void QtOsgWidget::paintGL()
  {
    makeCurrent();

	  if (m_isFirstFrame)
    {
      m_isFirstFrame = false;
      m_graphicsWindow->setDefaultFboId(defaultFramebufferObject());
    }

    // m_overlayCompositor->renderVirtualOverlays();
    m_viewer->frame();

    doneCurrent();
  }

  void QtOsgWidget::resizeGL(int width, int height)
  {
    m_graphicsWindow->getEventQueue()->windowResize(x(), y(), width, height);
    m_graphicsWindow->resized(x(), y(), width, height);

    m_overlayProjection->setMatrix(
            osg::Matrix::ortho2D(0.0, static_cast<double>(width) - 1.0, static_cast<double>(height) - 1.0, 0.0));

    m_view->updateResolution(osg::Vec2f(width, height), devicePixelRatio());
  }

  void QtOsgWidget::paintEvent(QPaintEvent* paintEvent)
  {
    paintGL();
  }

  void QtOsgWidget::keyPressEvent(QKeyEvent* event)
  {
    QOpenGLWidget::keyPressEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    handleEvent([keyData](osgGA::EventQueue* queue)
    {
      return queue->keyPress(osgGA::GUIEventAdapter::KeySymbol(*keyData));
    });
  }

  void QtOsgWidget::keyReleaseEvent(QKeyEvent* event)
  {
    QOpenGLWidget::keyReleaseEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    handleEvent([keyData](osgGA::EventQueue* queue)
    {
      return queue->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
    });
  }

  void QtOsgWidget::mouseMoveEvent(QMouseEvent* event)
  {
    QOpenGLWidget::mouseMoveEvent(event);

    const auto pixelRatio = devicePixelRatio();

    handleEvent([event, pixelRatio](osgGA::EventQueue* queue)
    {
      return queue->mouseMotion(static_cast<float>(event->x() * pixelRatio),
          static_cast<float>(event->y() * pixelRatio));
    });
  }

  void QtOsgWidget::mousePressEvent(QMouseEvent* event)
  {
    QOpenGLWidget::mousePressEvent(event);

    const auto pixelRatio = devicePixelRatio();

    handleEvent([event, pixelRatio](osgGA::EventQueue* queue)
    {
      return queue->mouseButtonPress(static_cast<float>(event->x() * pixelRatio),
          static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
    });
  }

  void QtOsgWidget::mouseReleaseEvent(QMouseEvent* event)
  {
    QOpenGLWidget::mouseReleaseEvent(event);

    const auto pixelRatio = devicePixelRatio();

    handleEvent([event, pixelRatio](osgGA::EventQueue* queue)
    {
        return queue->mouseButtonRelease(static_cast<float>(event->x() * pixelRatio),
            static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
    });
  }

  void QtOsgWidget::wheelEvent(QWheelEvent* event)
  {
    QOpenGLWidget::wheelEvent(event);

    event->accept();

    const auto delta  = event->angleDelta();
    const auto motion = (delta.y() > 0) ? osgGA::GUIEventAdapter::SCROLL_UP : osgGA::GUIEventAdapter::SCROLL_DOWN;

    handleEvent([motion](osgGA::EventQueue* queue)
    {
      return queue->mouseScroll(motion);
    });
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

  void QtOsgWidget::handleEvent(const EventHandlerFunc& handlerFunc) const
  {
    handlerFunc(m_graphicsWindow->getEventQueue());
  }

  void QtOsgWidget::setupVirtualOverlayNodes()
  {
    m_overlayProjection = new osg::Projection();
    m_overlayProjection->addChild(m_overlayCompositor);

    auto stateSet = m_overlayProjection->getOrCreateStateSet();
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setRenderBinDetails(10, "RenderBin");
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateSet->setAttributeAndModes(new osg::BlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA), osg::StateAttribute::ON);

    getView()->getCamera(osgHelper::View::CameraType::Screen)->addChild(m_overlayProjection);
  }
}
