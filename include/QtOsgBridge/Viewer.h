#pragma once

#include <osgViewer/CompositeViewer>

namespace QtOsgBridge
{
    class Viewer : public osgViewer::CompositeViewer
    {
    public:
      Viewer();
      ~Viewer() override;

    };
}