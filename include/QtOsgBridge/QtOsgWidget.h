#pragma once

#include <osgHelper/View.h>
#include <osgHelper/Camera.h>

#include <osgViewer/CompositeViewer>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

namespace QtOsgBridge
{
  using GLWidgetBase = QOpenGLWidget;

  class QtOsgWidget : public GLWidgetBase , protected QOpenGLFunctions
  {
    Q_OBJECT

  public:
    enum class UpdateMode
    {
      OnInputEvent,
      OnTimerEvent
    };

    enum class ViewType
    {
      Scene,
      //Screen,
      _Count
    };

    QtOsgWidget(QWidget* parent = nullptr);

    void setUpdateMode(UpdateMode mode);
    void setTargetFps(int fps);

    osg::ref_ptr<osgHelper::View>   getView(ViewType type = ViewType::Scene) const;
    osg::ref_ptr<osgHelper::Camera> getCamera(ViewType type = ViewType::Scene) const;

  protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;

    void paintEvent(QPaintEvent* paintEvent) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    bool event(QEvent* event) override;

  private:
    struct RenderLayer
    {
      osg::ref_ptr<osgViewer::CompositeViewer>        viewer;
      osg::ref_ptr<osgHelper::View>                   view;
      //osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> graphics;
      //osg::ref_ptr<osg::GraphicsContext>              context;
    };

    using RenderLayerList  = std::vector<RenderLayer>;
    using EventHandlerFunc = std::function<osgGA::GUIEventAdapter*(osgGA::EventQueue*)>;

    RenderLayerList                                 m_renderLayers;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_graphicsWindow;

    QTimer     m_updateTimer;
    UpdateMode m_updateMode;

    bool m_isFirstFrame;

    void handleEvent(const EventHandlerFunc& handlerFunc) const;

  };
}