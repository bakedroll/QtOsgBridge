#pragma once

#include "QtOsgBridge/Viewer.h"

#include <QOpenGLWidget>
#include <QTimer>

namespace QtOsgBridge
{
  class QtOsgWidget : public QOpenGLWidget
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

    osg::ref_ptr<Viewer>& getViewer();
    osg::ref_ptr<osgViewer::View>& getView();

  protected:
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
    void onResize(int width, int height) const;

    osg::ref_ptr<Viewer>                            m_viewer;
    osg::ref_ptr<osgViewer::View>                   m_view;
    osg::ref_ptr<osg::Camera>                       m_camera;
    osg::ref_ptr<osgViewer::GraphicsWindowEmbedded> m_graphicsWindow;

    QTimer     m_updateTimer;
    UpdateMode m_updateMode;
  };
}