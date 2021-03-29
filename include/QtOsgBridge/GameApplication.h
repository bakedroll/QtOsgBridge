#pragma once

#include <osgHelper/ioc/Injector.h>
#include <osgHelper/ioc/InjectionContainer.h>
#include <osgHelper/SimulationCallback.h>
#include <osgHelper/Observable.h>

#include <functional>

namespace QtOsgBridge
{
  class GameApplication : public osgHelper::SimulationCallback
	{
  public:
    GameApplication();
    ~GameApplication() override;

    osgHelper::Signal& onEndGameSignal();

  protected:
    virtual void initialize(osgHelper::ioc::Injector& injector);
    virtual void deinitialize();

    virtual void onException(const std::string& message);
    int safeExecute(std::function<int()> func);

    virtual void registerComponents(osgHelper::ioc::InjectionContainer& container);
    void registerEssentialComponents();

    virtual int mainloop() = 0;

    osgHelper::ioc::InjectionContainer& container();

	private:
    osgHelper::ioc::InjectionContainer m_container;
    osgHelper::Signal                  m_endGameSignal;
  };
}
