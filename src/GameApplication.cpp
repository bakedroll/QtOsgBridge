#include "QtOsgBridge/GameApplication.h"

#include <osgHelper/GameException.h>

#include <chrono>
#include <future>

#include <osgHelper/ShaderFactory.h>
#include <osgHelper/TextureFactory.h>
#include <osgHelper/ResourceManager.h>
#include <osgHelper/LogManager.h>

using namespace osg;
using namespace std;

namespace QtOsgBridge
{

  GameApplication::GameApplication() = default;
  GameApplication::~GameApplication() = default;

  void GameApplication::onException(const std::string& message)
  {
  }

  int GameApplication::safeExecute(std::function<int()> func)
  {
    try
    {
      return func();
    }
    catch (osgHelper::GameException& e)
    {
      OSGG_LOG_FATAL(std::string("Exception: ") + e.getMessage());
      onException(e.getMessage());
    }
    catch (exception& e)
    {
      OSGG_LOG_FATAL(std::string("Exception: ") + std::string(e.what()));
      onException(e.what());
    }

    return -1;
  }

  void GameApplication::initialize(osgHelper::ioc::Injector& injector)
  {
  }

  void GameApplication::deinitialize()
  {
  }

  void GameApplication::registerComponents(osgHelper::ioc::InjectionContainer& container)
  {
  }

  void GameApplication::registerEssentialComponents()
  {
    m_container.registerSingletonType<osgHelper::ShaderFactory>();
    m_container.registerSingletonType<osgHelper::ResourceManager>();
    m_container.registerSingletonType<osgHelper::TextureFactory>();
  }

  osgHelper::ioc::InjectionContainer& GameApplication::container()
  {
    return m_container;
  }
}
