#pragma once

#include <QtOsgBridge/MainWindow.h>

#include <libQtGame/AbstractGameState.h>
#include <libQtGame/KeyboardMouseEventFilter.h>

#include <QPointer>

namespace QtOsgBridge
{

class EventProcessingState : public libQtGame::AbstractGameState
{
public:
  EventProcessingState(osgHelper::ioc::Injector& injector);

  virtual void onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data);

  bool eventFilter(QObject* object, QEvent* event) override;

  QPointer<libQtGame::KeyboardMouseEventFilter> eventHandler() const;

protected:
  virtual void onResizeEvent(QResizeEvent* event);

  virtual bool onKeyEvent(QKeyEvent* event);
  virtual bool onMouseEvent(QMouseEvent* event);
  virtual bool onWheelEvent(QWheelEvent* event);

  virtual void onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin);
  virtual void onDragMove(
    Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position, const osg::Vec2f& change);
  virtual void onDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position);

private Q_SLOTS:
  void forwardKeyEvent(QKeyEvent* event, bool& accepted);
  void forwardMouseEvent(QMouseEvent* event, bool& accepted);
  void forwardWheelEvent(QWheelEvent* event, bool& accepted);

  void forwardDragBegin(Qt::MouseButton button, const osg::Vec2f& origin);
  void forwardDragMove(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position, const osg::Vec2f& change);
  void forwardDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position);

private:
  QPointer<libQtGame::KeyboardMouseEventFilter> m_eventHandler;

};

}
