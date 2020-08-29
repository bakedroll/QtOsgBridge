#include <QtOsgBridge/Viewer.h>

namespace QtOsgBridge
{
  Viewer::Viewer()
    : osgViewer::CompositeViewer()
  {
  }

  Viewer::~Viewer() = default;

}
