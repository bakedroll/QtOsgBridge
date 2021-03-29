#pragma once

#include <QWidget>

#include <osg/Group>

#include <memory>

namespace QtOsgBridge
{
  class VirtualOverlay;

  class OverlayCompositor : public osg::Group
  {
  public:
    OverlayCompositor(QWidget* parent);
    ~OverlayCompositor();

    void addVirtualOverlay(VirtualOverlay* overlay);
    void removeVirtualOverlay(VirtualOverlay* overlay);

    void clear();

    void renderVirtualOverlays();

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

  };
}