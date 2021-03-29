#pragma once

#include <QtOsgBridge/AbstractEventState.h>

#include <osg/Node>

namespace QtOsgBridge
{

class EmptyNode : public osg::Node
{
public:
  EmptyNode(osgHelper::ioc::Injector& injector)
  {
  }
};

template <typename TSceneNode = EmptyNode, typename TScreenNode = EmptyNode>
class EventStateBase : public AbstractEventState
{
public:
  EventStateBase(osgHelper::ioc::Injector& injector)
    : AbstractEventState(injector)
  {
    if (typeid(TSceneNode) != typeid(EmptyNode))
    {
      m_sceneNode = injector.inject<TSceneNode>();
    }

    if (typeid(TScreenNode) != typeid(EmptyNode))
    {
      m_screenNode = injector.inject<TScreenNode>();
    }
  }

  osg::ref_ptr<osg::Node> getSceneNode() const override
  {
    return m_sceneNode;
  }

  osg::ref_ptr<osg::Node> getScreenNode() const override
  {
    return m_screenNode;
  }

private:
  osg::ref_ptr<TSceneNode>  m_sceneNode;
  osg::ref_ptr<TScreenNode> m_screenNode;

};

}