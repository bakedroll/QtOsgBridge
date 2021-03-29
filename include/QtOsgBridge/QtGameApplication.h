#pragma once

#include <QtOsgBridge/Multithreading.h>
#include <QtOsgBridge/GameApplication.h>
#include <QtOsgBridge/AbstractEventState.h>

#include <memory>

namespace QtOsgBridge
{
  class QtGameApplication : public Multithreading, public GameApplication
  {
    Q_OBJECT

  public:
    QtGameApplication(int& argc, char** argv);
    ~QtGameApplication();

    template <typename TState>
    int runGame()
    {
      auto& c = container();

      registerComponents(c);
      osgHelper::ioc::Injector injector(c);
      m_injector = &injector;

      initialize(*m_injector);

      auto state = m_injector->inject<TState>();
      if (!state.valid())
      {
        assert(false && "TState has to be an AbstractEventState.");
        return -1;
      }

      return runGame(state);
    }

    void action(osg::Object* object, osg::Object* data, double simTime, double timeDiff) override;
    bool notify(QObject *receiver, QEvent *event) override;

  protected:
    int runGame(const osg::ref_ptr<AbstractEventState>& initialState);

    void prepareEventState(const osg::ref_ptr<AbstractEventState>& state);
    void deinitialize() override;

    int mainloop() override;
    void onException(const std::string& message) override;

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

    osgHelper::ioc::Injector* m_injector;

  };
}
