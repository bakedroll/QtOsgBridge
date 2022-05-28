#pragma once

#include <QtOsgBridge/KeyboardMouseEventFilterBase.h>

#include <QKeyEvent>

#include <osg/Vec2f>

#include <optional>

namespace QtOsgBridge
{

class KeyboardMouseEventFilter : public QObject,
                                 public KeyboardMouseEventFilterBase
{
  Q_OBJECT

public:
  KeyboardMouseEventFilter();
  ~KeyboardMouseEventFilter() override;

  bool eventFilter(QObject* object, QEvent* event) override;

};

}