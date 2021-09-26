#include "QtOsgBridgeApplication.h"
#include "InitialState.h"
#include "LoadingInitialState.h"

QtOsgBridgeApplication::QtOsgBridgeApplication(int& argc, char** argv)
  : QtGameApplication(argc, argv)
{
}

void QtOsgBridgeApplication::registerComponents(osgHelper::ioc::InjectionContainer& container)
{
  registerEssentialComponents(container);

  container.registerType<LoadingInitialState>();
  container.registerType<InitialState>();
  container.registerType<State1>();
  container.registerType<State2>();
  container.registerType<osgHelper::ppu::DOF>();
  container.registerType<osgHelper::ppu::FXAA>();
  container.registerType<osgHelper::ppu::HDR>();
}
