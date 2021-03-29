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

  GameApplication::GameApplication()
    : SimulationCallback()
  {
  }

  GameApplication::~GameApplication() = default;

  osgHelper::Signal& GameApplication::onEndGameSignal()
  {
    return m_endGameSignal;
  }

  /*void GameApplication::action(osg::Object* object, osg::Object* data, double simTime, double timeDiff)
  {
    if (m->gameEnded)
    {
      return;
    }

      state->setSimulationTime(simTime);
      state->setFrameTime(timeDiff);

        if (!initialized)
        {
          if (state->isLoadingState())
          {
            if (!m->isLoading)
            {
              m->isLoading = true;
            }

            ref_ptr<GameLoadingState> loadingState = dynamic_cast<GameLoadingState*>(state.get());

            AbstractGameState::AbstractGameStateList nextStates;
            loadingState->getNextStates(*m_injector, nextStates);

            for (osgGaming::GameState::AbstractGameStateList::iterator it = nextStates.begin(); it != nextStates.end(); ++it)
              (*it)->setInjector(*m_injector);

            m->prepareStateWorldAndHud(nextStates);

            ref_ptr<AbstractGameState> nextState = *nextStates.begin();

            View::Ptr view = m->viewer->getGamingView(0);
            if (!view)
              assert(false);
           
            m->loadingThreadFuture = async(launch::async,
              &GameLoadingState::loading_thread,
              loadingState.get(),
              nextState->getWorld(view),
              nextState->getHud(view),
              getDefaultGameSettings());

          }
          else
          {
            if (m->isLoading)
            {
              m->isLoading = false;
              resetTimeDiff();
            }
          }
        }

 
        state.release();
      }
    }
    else
    {
      m->gameEnded = true;
    }

    traverse(object, data);
  }*/

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
