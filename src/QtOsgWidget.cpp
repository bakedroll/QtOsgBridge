#include <QtOsgBridge/QtOsgWidget.h>

#include <osgHelper/Macros.h>

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
    : GLWidgetBase(parent)
    , m_updateMode(UpdateMode::OnInputEvent)
    , m_isFirstFrame(true)
  {
    //setTextureFormat(GL_RGBA16F_ARB);



    //setTextureFormat(GL_RGB16F);

    //auto surfaceFormat = format();
    //auto a = surfaceFormat.redBufferSize();

    //auto profile = surfaceFormat.profile();
    //auto type = surfaceFormat.renderableType();
    //auto swap = surfaceFormat.swapBehavior();

    /*surfaceFormat.setAlphaBufferSize(0);
    surfaceFormat.setRedBufferSize(16);
    surfaceFormat.setGreenBufferSize(16);
    surfaceFormat.setBlueBufferSize(16);

    setFormat(surfaceFormat);*/

    //auto c = context();
    //auto fbo = c->defaultFramebufferObject();

    const auto w          = width();
    const auto h          = height();
    const auto pixelRatio = devicePixelRatio();

    const auto numLayers = static_cast<int>(ViewType::_Count);
    m_renderLayers.resize(numLayers);

    /*const auto traits    = new osg::GraphicsContext::Traits();
    traits->alpha        = 16;
    traits->red          = 16;
    traits->green        = 16;
    traits->blue         = 16;
    traits->depth        = 8;
    traits->doubleBuffer = false;
    traits->format       = GL_RGBA16F_ARB;
    traits->x            = x();
    traits->y            = y();
    traits->width        = w;
    traits->height       = h;*/
    //traits->setInheritedWindowPixelFormat = true;

    m_graphicsWindow = new osgViewer::GraphicsWindowEmbedded(0, 0, w * pixelRatio, h * pixelRatio);
    //m_graphicsWindow = new osgViewer::GraphicsWindowEmbedded(traits);
    //m_graphicsWindow = new osgViewer::GraphicsWindowEmbedded();

    /*QSurfaceFormat format;
    format.setVersion(2, 1);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);

    format.setRedBufferSize(16);
    format.setGreenBufferSize(16);
    format.setBlueBufferSize(16);
    format.setAlphaBufferSize(16);
    format.setColorSpace(QSurfaceFormat::ColorSpace::sRGBColorSpace);
    format.setDepthBufferSize(8);
    format.setSwapBehavior(QSurfaceFormat::SwapBehavior::SingleBuffer);
    format.setRenderableType(QSurfaceFormat::RenderableType::OpenGL);

    setFormat(format);*/

    /*QTimer::singleShot(0, [this, format]()
    {
      auto c = context();
      c->setFormat(format);
    });*/

    for (auto i=0; i<numLayers; i++)
    {
      RenderLayer layer;

      layer.view = new osgHelper::View();

      // layer.graphics = new osgViewer::GraphicsWindowEmbedded(traits);

      layer.view->getSceneCamera()->setGraphicsContext(m_graphicsWindow);
      layer.view->updateViewport(0, 0, w, h, pixelRatio);

      m_graphicsWindow->getEventQueue()->syncWindowRectangleWithGraphicsContext();

      layer.viewer = new osgViewer::CompositeViewer();
      layer.viewer->addView(layer.view);
      layer.viewer->setThreadingModel(osgViewer::CompositeViewer::SingleThreaded);
      layer.viewer->setReleaseContextAtEndOfFrameHint(false);

      //osgViewer::Viewer::Windows windows;
      //layer.viewer->getWindows(windows);

      layer.viewer->realize();

      //layer.view->setOpenGLMakeContextCurrentFunction(std::bind(&QtOsgWidget::makeCurrent, this),
      //                                                std::bind(&QtOsgWidget::doneCurrent, this));

      //
      //
      //
      //layer.view->setOpenGLMakeContextCurrentFunction(
      //        std::bind(&osgViewer::GraphicsWindowEmbedded::makeCurrent, m_graphicsWindow.get()));

      //layer.view->setOpenGLMakeContextCurrentFunction(
      //  std::bind(&osgViewer::GraphicsWindowEmbedded::makeCurrent, layer.graphics.get()));

      m_renderLayers[i] = layer;
    }

    /*const auto screenStage   = m_renderLayers[static_cast<int>(ViewType::Screen)];
    auto       sceneStageCam = screenStage.view->getSceneCamera();

    sceneStageCam->setClearMask(GL_DEPTH_BUFFER_BIT);
    sceneStageCam->setProjectionMode(osgHelper::Camera::ProjectionMode::Ortho2D);

    auto stateSet = screenStage.view->getRootGroup()->getOrCreateStateSet();
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    stateSet->setRenderBinDetails(10, "RenderBin");*/

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

    return m_renderLayers[index].view->getSceneCamera();
  }

  void QtOsgWidget::initializeGL()
  {
    initializeOpenGLFunctions();
  }

  void QtOsgWidget::paintGL()
  {
    //makeCurrent();

    //m_graphicsWindow->makeCurrent();

	  if (m_isFirstFrame)
    {
      m_isFirstFrame = false;
      m_graphicsWindow->setDefaultFboId(defaultFramebufferObject());
    }

    const auto numViewers = static_cast<int>(ViewType::_Count);
    for (auto i = 0; i < numViewers; i++)
    {
      auto& layer = m_renderLayers[i];

      //layer.graphics->makeCurrent();
      layer.viewer->frame();
      //layer.graphics->swapBuffers();
    }

    //doneCurrent();
    //
    //m_graphicsWindow->swapBuffers();
  }

  void QtOsgWidget::resizeGL(int width, int height)
  {
    m_graphicsWindow->getEventQueue()->windowResize(x(), y(), width, height);
    m_graphicsWindow->resized(x(), y(), width, height);

    const auto numViewers = static_cast<int>(ViewType::_Count);
    for (auto i=0; i<numViewers; i++)
    {
      auto view     = m_renderLayers[i].view;
      //auto graphics = m_renderLayers[i].graphics;

      //graphics->getEventQueue()->windowResize(x(), y(), width, height);
      //graphics->resized(x(), y(), width, height);

      view->updateResolution(osg::Vec2f(width, height), devicePixelRatio());
      view->getSceneCamera()->updateResolution(osg::Vec2i(width, height));
    }
  }

  void QtOsgWidget::paintEvent(QPaintEvent* paintEvent)
  {
    paintGL();
  }

  void QtOsgWidget::keyPressEvent(QKeyEvent* event)
  {
      GLWidgetBase::keyPressEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    handleEvent([keyData](osgGA::EventQueue* queue)
    {
      return queue->keyPress(osgGA::GUIEventAdapter::KeySymbol(*keyData));
    });
  }

  void QtOsgWidget::keyReleaseEvent(QKeyEvent* event)
  {
      GLWidgetBase::keyReleaseEvent(event);

    const auto keyString = event->text();
    const auto keyData   = keyString.toLocal8Bit().data();

    handleEvent([keyData](osgGA::EventQueue* queue)
    {
      return queue->keyRelease(osgGA::GUIEventAdapter::KeySymbol(*keyData));
    });
  }

  void QtOsgWidget::mouseMoveEvent(QMouseEvent* event)
  {
      GLWidgetBase::mouseMoveEvent(event);

    const auto pixelRatio = devicePixelRatio();

    handleEvent([event, pixelRatio](osgGA::EventQueue* queue)
    {
      return queue->mouseMotion(static_cast<float>(event->x() * pixelRatio),
          static_cast<float>(event->y() * pixelRatio));
    });
  }

  void QtOsgWidget::mousePressEvent(QMouseEvent* event)
  {
    GLWidgetBase::mousePressEvent(event);

    const auto pixelRatio = devicePixelRatio();

    handleEvent([event, pixelRatio](osgGA::EventQueue* queue)
    {
      return queue->mouseButtonPress(static_cast<float>(event->x() * pixelRatio),
          static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
    });
  }

  void QtOsgWidget::mouseReleaseEvent(QMouseEvent* event)
  {
      GLWidgetBase::mouseReleaseEvent(event);

    const auto pixelRatio = devicePixelRatio();

    handleEvent([event, pixelRatio](osgGA::EventQueue* queue)
    {
        return queue->mouseButtonRelease(static_cast<float>(event->x() * pixelRatio),
            static_cast<float>(event->y() * pixelRatio), getOsgMouseButton(event));
    });
  }

  void QtOsgWidget::wheelEvent(QWheelEvent* event)
  {
      GLWidgetBase::wheelEvent(event);

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

  void QtOsgWidget::handleEvent(const EventHandlerFunc& handlerFunc) const
  {
    handlerFunc(m_graphicsWindow->getEventQueue());


    /*bool handled = false;
    std::for_each(m_renderLayers.rbegin(), m_renderLayers.rend(), [&handled, &handlerFunc](const RenderLayer& layer)
    {
      if (handled)
      {
        return;
      }

      const auto adapter = handlerFunc(layer.graphics->getEventQueue());
      if (adapter && adapter->getHandled())
      {
        handled = true;
      }
    });*/
  }
}
