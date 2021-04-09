#pragma once

#include <QMainWindow>
#include <memory>

#include <QPointer>

#include <QtOsgBridge/QtOsgWidget.h>

namespace QtOsgBridge
{
  class MainWindow : public QMainWindow
  {
  public:
    MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

    QPointer<QtOsgWidget> getViewWidget() const;

    void shutdown();

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

  };
}