#pragma once

#include <QtOsgBridge/QtGameApplication.h>

class QtOsgBridgeApplication : public QtOsgBridge::QtGameApplication
{
public:
  QtOsgBridgeApplication(int& argc, char** argv);

protected:
	void registerComponents(osgHelper::ioc::InjectionContainer& container) override;

};