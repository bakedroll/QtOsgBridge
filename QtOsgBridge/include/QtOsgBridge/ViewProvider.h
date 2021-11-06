#pragma once

#include <osgHelper/ioc/Injector.h>

#include <QtOsgBridge/IViewProvider.h>

namespace QtOsgBridge
{

class ViewProvider : public IViewProvider
{
public:
  ViewProvider(osgHelper::ioc::Injector& injector);
  ~ViewProvider() override;

  osg::ref_ptr<osgHelper::View> getView() const override;
  void setView(const osg::ref_ptr<osgHelper::View>& view) override;

private:
  osg::ref_ptr<osgHelper::View> m_view;

};

}