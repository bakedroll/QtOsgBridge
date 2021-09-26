#include <QtOsgBridge/LoadingThreadContext.h>

#include <QtUtilsLib/MultithreadedApplication.h>

namespace QtOsgBridge
{

LoadingThreadContext::LoadingThreadContext(LoadingFunc func)
  : QObject()
  , m_func(func)
{
}

void LoadingThreadContext::doWork()
{
  m_func();

  QtUtilsLib::MultithreadedApplication::executeInUiAsync([this]()
  {
    Q_EMIT workDone();
  });
}

}
