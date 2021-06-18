#include <QtOsgBridge/GameUpdateCallback.h>

namespace QtOsgBridge
{

GameUpdateCallback::GameUpdateCallback(UpdateFunc func)
  : osgHelper::SimulationCallback()
  , m_func(func)
{
}

void GameUpdateCallback::action(const SimulationData& data)
{
  m_func(data);
}

}
