#include <QtOsgBridge/LoadingThreadContext.h>

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
}

}
