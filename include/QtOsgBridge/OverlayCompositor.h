#pragma once

#include <QObject>

#include <osg/Switch>

#include <QPointer>

namespace QtOsgBridge
{
  class VirtualOverlay;

  class OverlayCompositor : public QObject, public osg::Switch
  {
    Q_OBJECT

  public:
    OverlayCompositor();
    ~OverlayCompositor();

    void addVirtualOverlay(const QPointer<VirtualOverlay>& overlay);
    void clear();

    void renderVirtualOverlays();

  private Q_SLOTS:
    void onVirtualOverlayToggled(const QPointer<VirtualOverlay>& overlay, bool enabled);
    void onVirtualOverlayDestroyed(QObject* object);

  private:
    struct OverlayData
    {
      std::vector<QMetaObject::Connection> connections;
      osg::ref_ptr<osg::Node>              rootNode;
    };

    using OverlayConnectionMap = std::map<QPointer<VirtualOverlay>, OverlayData>;

    OverlayConnectionMap m_overlays;

  };
}
