#pragma once

#include <QtOsgBridge/QtGameApplication.h>

#include <osgHelper/ioc/InjectionContainer.h>

class QtOsgBridgeApplication : public QtOsgBridge::QtGameApplication
{
  Q_OBJECT

public:
  QtOsgBridgeApplication(int& argc, char** argv);

protected:
	void registerComponents(osgHelper::ioc::InjectionContainer& container) override;

};
