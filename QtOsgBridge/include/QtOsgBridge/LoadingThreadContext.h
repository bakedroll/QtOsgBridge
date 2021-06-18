#pragma once

#include <QObject>

namespace QtOsgBridge
{

class LoadingThreadContext : public QObject
{
  Q_OBJECT

public:
  using LoadingFunc = std::function<void()>;

  explicit LoadingThreadContext(LoadingFunc func);

Q_SIGNALS:
  void workDone();

public Q_SLOTS:
  void doWork();

private:
  LoadingFunc m_func;

};

}
