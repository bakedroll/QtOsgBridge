#include "QtOsgBridge/Multithreading.h"

#include <QThread>

namespace QtOsgBridge
{

Multithreading::Multithreading(int& argc, char** argv) : QApplication(argc, argv)
{
  qRegisterMetaType<std::function<void()>>("std::function<void()>");

  connect(this, &Multithreading::executeFunctionAsync, this, &Multithreading::executeFunction, Qt::QueuedConnection);

  connect(this, &Multithreading::executeFunctionBlocking, this, &Multithreading::executeFunction,
          Qt::BlockingQueuedConnection);
}

Multithreading::~Multithreading() = default;

void Multithreading::executeInUiBlocking(std::function<void()> func)
{
  if (isInUIThread())
  {
    func();
    return;
  }

  Q_EMIT static_cast<Multithreading*>(qApp)->executeFunctionBlocking(func);
}

void Multithreading::executeInUiAsync(std::function<void()> func, bool executeInUIThread)
{
  if (isInUIThread())
  {
    func();
    return;
  }

  Q_EMIT static_cast<Multithreading*>(qApp)->executeFunctionAsync(func);
}

bool Multithreading::isInUIThread()
{
  return (!qApp || (QThread::currentThread() == qApp->thread()));
}

void Multithreading::executeFunction(std::function<void()> func) const
{
  func();
}

}  // namespace QtOsgBridge