#pragma once

#include <QtOsgBridge/MainWindow.h>

#include <osgHelper/ioc/Injector.h>
#include <osgHelper/Macros.h>

#include <osg/Node>

#include <QObject>
#include <QPointer>

namespace QtOsgBridge
{

class AbstractEventState : public QObject, public osg::Referenced
{
  Q_OBJECT

public:
  enum class StateRequestType
  {
    NewState,
    ExitState,
    ExitPastStates
  };

  AbstractEventState(osgHelper::ioc::Injector& injector)
    : m_injector(&injector)
  {
  }

  virtual ~AbstractEventState() = default;

  virtual bool isLoadingState() const = 0;
  virtual void initialize(MainWindow* mainWindow) {}

  template <typename TState>
  void requestStateEvent(StateRequestType type)
  {
    const auto state = m_injector->inject<TState>();
    assert_return(state.valid());

    Q_EMIT forwardStateRequest(type, QPointer<AbstractEventState>(m_injector->inject<TState>().get()));
  }

Q_SIGNALS:
  void forwardStateRequest(StateRequestType type, QPointer<AbstractEventState> state);

private:
  osgHelper::ioc::Injector* m_injector;

};

}