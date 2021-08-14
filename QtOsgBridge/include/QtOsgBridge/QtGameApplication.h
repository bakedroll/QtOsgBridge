#pragma once

#include <QtOsgBridge/Multithreading.h>
#include <QtOsgBridge/AbstractEventState.h>
#include <QtOsgBridge/GameUpdateCallback.h>

#include <osgHelper/GameApplication.h>
#include <osgHelper/SimulationCallback.h>

#include <memory>

#include <QMetaObject>

namespace QtOsgBridge
{
  class QtGameApplication : public Multithreading, public osgHelper::GameApplication
  {
    Q_OBJECT

  public:
    QtGameApplication(int& argc, char** argv);
    ~QtGameApplication();

    template <typename TState>
    int runGame()
    {
      setupIOC();

      if (!injectPushAndPrepareState<TState>())
      {
        return -1;
      }

      return runGame();
    }

    bool notify(QObject *receiver, QEvent *event) override;

  protected:
    struct StateData
    {
      osg::ref_ptr<AbstractEventState>     state;
      std::vector<QMetaObject::Connection> connections;
    };

    int runGame();

    void prepareEventState(StateData& data);
    void onException(const std::string& message) override;

  private:
    using StateList = std::vector<StateData>;

    struct Impl;
    std::unique_ptr<Impl> m;

    StateList                        m_states;
    osg::ref_ptr<GameUpdateCallback> m_updateCallback;

    template <typename TState>
    bool injectPushAndPrepareState()
    {
      auto state = injector().inject<TState>();
      assert_return(state.valid(), false);

      pushAndPrepareState(state);

      return true;
    }

    void updateStates(const osgHelper::SimulationCallback::SimulationData& data);
    void pushAndPrepareState(const osg::ref_ptr<AbstractEventState>& state);
    void exitState(const osg::ref_ptr<AbstractEventState>& state);

  private Q_SLOTS:
    void onNewEventStateRequest(const osg::ref_ptr<AbstractEventState>& current,
                                AbstractEventState::NewEventStateMode   mode,
                                const osg::ref_ptr<AbstractEventState>& newState);
    void onExitEventStateRequest(const osg::ref_ptr<AbstractEventState>& current,
                                 AbstractEventState::ExitEventStateMode  mode);
  };

}
