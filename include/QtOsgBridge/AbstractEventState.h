#pragma once

#include <QtOsgBridge/MainWindow.h>

#include <osgHelper/ioc/Injector.h>
#include <osgHelper/Macros.h>

namespace QtOsgBridge
{

class AbstractEventState : public QObject,  public osg::Referenced
{
  Q_OBJECT

public:
  enum class NewEventStateMode
  {
    ExitCurrent,
    ContinueCurrent
  };

  enum class ExitEventStateMode
  {
    ExitCurrent,
    ExitAll
  };

  explicit AbstractEventState(osgHelper::ioc::Injector& injector);

  virtual ~AbstractEventState() = default;

  virtual bool isLoadingState() const = 0;
  virtual void onInitialize(MainWindow* mainWindow);
  virtual void onExit();

  template <typename TState>
  void requestNewEventState(NewEventStateMode mode)
  {
    const auto state = m_injector->inject<TState>();
    assert_return(state.valid());

    Q_EMIT forwardNewEventStateRequest(this, mode, state);
  }

  void requestExitEventState(ExitEventStateMode mode);

Q_SIGNALS:
  void forwardNewEventStateRequest(const osg::ref_ptr<AbstractEventState>& current, NewEventStateMode mode,
                                   const osg::ref_ptr<AbstractEventState>& newState);
  void forwardExitEventStateRequest(const osg::ref_ptr<AbstractEventState>& current, ExitEventStateMode mode);

private:
  osgHelper::ioc::Injector* m_injector;

};

}