#pragma once

#include <QtOsgBridge/AbstractEventState.h>

namespace QtOsgBridge
{

class EventProcessingState : public AbstractEventState
{
public:
  EventProcessingState(osgHelper::ioc::Injector& injector);

  bool isLoadingState() const override;
  bool eventFilter(QObject* object, QEvent* event) override;

protected:
  virtual bool onKeyEvent(QKeyEvent* event);
  virtual bool onMouseEvent(QMouseEvent* event);

};

}