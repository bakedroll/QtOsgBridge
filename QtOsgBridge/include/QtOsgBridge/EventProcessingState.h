#pragma once

#include <QtOsgBridge/AbstractEventState.h>

#include <QPointer>

namespace QtOsgBridge
{

class EventProcessingState : public AbstractEventState
{
public:
  EventProcessingState(osgHelper::ioc::Injector& injector);

  bool eventFilter(QObject* object, QEvent* event) override;

protected:
  virtual bool onKeyEvent(QKeyEvent* event);
  virtual bool onMouseEvent(QMouseEvent* event);
  virtual bool onWheelEvent(QWheelEvent* event);

  virtual void onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin);
  virtual void onDragMove(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position,
                          const osg::Vec2f& change);
  virtual void onDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position);

  bool isKeyDown(Qt::Key key) const;
  bool isMouseButtonDown(Qt::MouseButton button) const;

  void setCaptureMouse(bool on);

private:
  struct DragState
  {
    bool       moved;
    osg::Vec2f origin;
    osg::Vec2f lastPos;
  };

  std::map<Qt::MouseButton, DragState> m_dragStates;

  std::map<Qt::MouseButton, bool> m_isMouseDown;
  std::map<Qt::Key, bool>         m_isKeyDown;

  bool   m_isMouseCaptured;
  QPoint m_capturedMousePos;

};

}