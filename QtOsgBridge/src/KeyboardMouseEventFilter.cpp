#include <QtOsgBridge/KeyboardMouseEventFilter.h>

#include <QMouseEvent>
#include <QCursor>

#include <utilsLib/Utils.h>

namespace QtOsgBridge
{

KeyboardMouseEventFilter::KeyboardMouseEventFilter() :
  QObject(),
  KeyboardMouseEventFilterBase()
{
}

KeyboardMouseEventFilter::~KeyboardMouseEventFilter() = default;

bool KeyboardMouseEventFilter::eventFilter(QObject* object, QEvent* event)
{
  if (filterKeyboardMouseEvents(object, event))
  {
    return true;
  }

  return QObject::eventFilter(object, event);
}
}
