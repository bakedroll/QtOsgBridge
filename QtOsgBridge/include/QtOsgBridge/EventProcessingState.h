#pragma once

#include <QtOsgBridge/AbstractEventState.h>
#include <QtOsgBridge/KeyboardMouseEventFilterBase.h>

#include <optional>

namespace QtOsgBridge
{

class EventProcessingState : public AbstractEventState,
                             public KeyboardMouseEventFilterBase
{
public:
  EventProcessingState(osgHelper::ioc::Injector& injector);

  bool eventFilter(QObject* object, QEvent* event) override;

};

}
