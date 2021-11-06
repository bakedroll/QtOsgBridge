#pragma once

#include <osg/ref_ptr>
#include <osg/Referenced>

#include <osgHelper/View.h>

namespace QtOsgBridge
{

class IViewProvider : public osg::Referenced
{
public:
  IViewProvider() = default;
  virtual ~IViewProvider() = default;

  virtual osg::ref_ptr<osgHelper::View> getView() const = 0;
  virtual void setView(const osg::ref_ptr<osgHelper::View>& view) = 0;

};

}
