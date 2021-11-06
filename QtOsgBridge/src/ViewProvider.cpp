#include "QtOsgBridge/ViewProvider.h"

namespace QtOsgBridge
{

ViewProvider::ViewProvider(osgHelper::ioc::Injector& injector)
  : IViewProvider()
{
}

ViewProvider::~ViewProvider() = default;

osg::ref_ptr<osgHelper::View> ViewProvider::getView() const
{
  return m_view;
}

void ViewProvider::setView(const osg::ref_ptr<osgHelper::View>& view)
{
  m_view = view;
}

}
