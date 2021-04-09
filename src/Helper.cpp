#include <QtOsgBridge/Helper.h>

#include <QWidget>

namespace QtOsgBridge
{

void Helper::deleteLayout(const QPointer<QLayout>& layout)
{
  QLayoutItem*          item = nullptr;
  QPointer<QLayout>     sublayout;
  QPointer<QWidget>     widget;

  while ((item = layout->takeAt(0)))
  {
    if ((sublayout = item->layout()) != nullptr)
    {
      deleteLayout(sublayout);
    }
    else if ((widget = item->widget()) != nullptr)
    {
      widget->hide();
      widget->deleteLater();
    }
    else
    {
      delete item;
    }
  }

  layout->deleteLater();
}

void Helper::deleteWidget(const QPointer<QWidget>& widget)
{
  if (widget->layout())
  {
    deleteLayout(widget->layout());
  }

  widget->setParent(nullptr);
  widget->deleteLater();
}

}