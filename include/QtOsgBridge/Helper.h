#pragma once

#include <QLayout>
#include <QPointer>

namespace QtOsgBridge
{

class Helper
{
public:
  static void deleteLayout(const QPointer<QLayout>& layout);
  static void deleteWidget(const QPointer<QWidget>& widget);
};

}
