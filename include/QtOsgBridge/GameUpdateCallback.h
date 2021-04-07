#pragma once

#include <functional>

#include <osgHelper/SimulationCallback.h>

namespace QtOsgBridge
{
  class GameUpdateCallback : public osgHelper::SimulationCallback
  {
  public:
    using UpdateFunc = std::function<void(const SimulationData&)>;

    GameUpdateCallback(UpdateFunc func);

	protected:
    void action(const SimulationData& data) override;

  private:
    UpdateFunc m_func;

  };
}