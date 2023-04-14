#include <QtOsgBridge/LoadingThreadContext.h>

#include <QtUtilsLib/Multithreading.h>

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

  QtUtilsLib::Multithreading::executeInUiAsync([this]()
  {
    Q_EMIT workDone();
  });
}

}
