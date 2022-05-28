#pragma once

#include <QKeyEvent>

#include <osg/Vec2f>

#include <optional>

namespace QtOsgBridge
{

class KeyboardMouseEventFilterBase
{
public:
  KeyboardMouseEventFilterBase();
  virtual ~KeyboardMouseEventFilterBase();

protected:
  bool filterKeyboardMouseEvents(QObject* object, QEvent* event);

  virtual bool onKeyEvent(QKeyEvent* event);
  virtual bool onMouseEvent(QMouseEvent* event);
  virtual bool onWheelEvent(QWheelEvent* event);

  virtual void onDragBegin(Qt::MouseButton button, const osg::Vec2f& origin);
  virtual void onDragMove(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position,
                          const osg::Vec2f& change);
  virtual void onDragEnd(Qt::MouseButton button, const osg::Vec2f& origin, const osg::Vec2f& position);

  bool isKeyDown(Qt::Key key) const;
  bool isMouseButtonDown(Qt::MouseButton button) const;
  bool isMouseDragging(const std::optional<Qt::MouseButton>& button = std::nullopt) const;

  void setCaptureMouse(bool on);

private:
  struct MouseDragData
  {
    Qt::MouseButton button;
    bool            moved;
    osg::Vec2f      origin;
    osg::Vec2f      lastPos;
  };

  std::optional<MouseDragData> m_mouseDragData;

  std::map<Qt::MouseButton, bool> m_isMouseDown;
  std::map<Qt::Key, bool>         m_isKeyDown;

  bool m_isMouseCaptured;
  QPoint m_capturedMousePos;

};

}