#pragma once

#include <QtOsgBridge/MainWindow.h>
#include <QtOsgBridge/Multithreading.h>

#include <osgHelper/ioc/Injector.h>
#include <osgHelper/Macros.h>
#include <osgHelper/SimulationCallback.h>

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

  using SimulationData = osgHelper::SimulationCallback::SimulationData;

  explicit AbstractEventState(osgHelper::ioc::Injector& injector);

  virtual ~AbstractEventState() = default;

  virtual void onInitialize(QPointer<MainWindow> mainWindow);
  virtual void onUpdate(const SimulationData& data);
  virtual void onExit();

  template <typename TState>
  void requestNewEventState(NewEventStateMode mode)
  {
    Multithreading::executeInUiAsync([this, mode]()
    {
      const auto state = m_injector->inject<TState>();
      assert_return(state.valid());

      Q_EMIT forwardNewEventStateRequest(this, mode, state);
    });
  }

  void requestExitEventState(ExitEventStateMode mode);
  void requestResetTimeDelta();

Q_SIGNALS:
  void forwardNewEventStateRequest(const osg::ref_ptr<AbstractEventState>& current, NewEventStateMode mode,
                                   const osg::ref_ptr<AbstractEventState>& newState);
  void forwardExitEventStateRequest(const osg::ref_ptr<AbstractEventState>& current, ExitEventStateMode mode);

  void forwardResetTimeDeltaRequest();

private:
  osgHelper::ioc::Injector* m_injector;

};

}