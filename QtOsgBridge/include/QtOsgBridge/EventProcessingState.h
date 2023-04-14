#pragma once

#include <QtOsgBridge/KeyboardMouseEventFilterBase.h>
#include <QtOsgBridge/MainWindow.h>

#include <libQtGame/AbstractGameState.h>

#include <QPointer>

#include <optional>

namespace QtOsgBridge
{

class EventProcessingState : public libQtGame::AbstractGameState,
                             public KeyboardMouseEventFilterBase
{
public:
  EventProcessingState(osgHelper::ioc::Injector& injector);

  virtual void onInitialize(QPointer<MainWindow> mainWindow, const SimulationData& data);

  bool eventFilter(QObject* object, QEvent* event) override;

protected:
  virtual void onResizeEvent(QResizeEvent* event);

};

}
