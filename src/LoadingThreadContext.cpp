#include <QtOsgBridge/LoadingThreadContext.h>
#include <QtOsgBridge/Multithreading.h>

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

  Multithreading::executeInUiAsync([this]()
  {
    Q_EMIT workDone();
  });
}

}
