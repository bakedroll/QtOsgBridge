#pragma once

#include <osgHelper/View.h>

#include <osgViewer/CompositeViewer>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>

namespace QtOsgBridge
{
  class QtOsgWidget : public QOpenGLWidget, protected QOpenGLFunctions
  {
    Q_OBJECT

  public:
    enum class UpdateMode
    {
      OnInputEvent,
      OnTimerEvent
    };

    QtOsgWidget(QWidget* parent = nullptr);

    void setUpdateMode(UpdateMode mode);
    void setTargetFps(int fps);

    osg::ref_ptr<osgHelper::View> getView() const;

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
    using EventHandlerFunc = std::function<osgGA::GUIEventAdapter*(osgGA::EventQueue*)>;

    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_graphicsWindow;
    osg::ref_ptr<osgViewer::CompositeViewer>        m_viewer;
    osg::ref_ptr<osgHelper::View>                   m_view;

    QTimer     m_updateTimer;
    UpdateMode m_updateMode;

    bool m_isFirstFrame;

    void handleEvent(const EventHandlerFunc& handlerFunc) const;

  };
}