#include <QtOsgBridge/OverlayCompositor.h>
#include <QtOsgBridge/VirtualOverlay.h>

#include <QtOsgBridge/Helper.h>

#include <osgHelper/LogManager.h>
#include <osgHelper/Macros.h>

namespace QtOsgBridge
{

OverlayCompositor::OverlayCompositor()
  : osg::Switch()
{
}

OverlayCompositor::~OverlayCompositor() = default;

void OverlayCompositor::addVirtualOverlay(const QPointer<VirtualOverlay>& overlay)
{
  if (m_overlays.count(overlay) > 0)
  {
    OSGH_LOG_WARN("Virtual overlay '" + overlay->objectName().toStdString() + "' already added");
    return;
  }

  addChild(overlay->getRootNode(), overlay->isVirtual());

  auto& data = m_overlays[overlay];

  data.rootNode = overlay->getRootNode();
  data.connections.push_back(
          connect(overlay, &VirtualOverlay::toggledIsVirtual, this, &OverlayCompositor::onVirtualOverlayToggled));
  data.connections.push_back(connect(overlay, &VirtualOverlay::toggledIsVirtualVisible, this,
                                     &OverlayCompositor::onVirtualOverlayVisibilityToggled));
  data.connections.push_back(
          connect(overlay, &QObject::destroyed, this, &OverlayCompositor::onVirtualOverlayDestroyed));
}

void OverlayCompositor::clear()
{
  for (const auto& overlay : m_overlays)
  {
    if (overlay.first->isVirtual())
    {
      removeChild(overlay.first->getRootNode());
    }

    overlay.first->setParent(nullptr);
    Helper::deleteWidget(overlay.first.data());
  }

  m_overlays.clear();
}

void OverlayCompositor::renderVirtualOverlays()
{
  for (const auto& overlay : m_overlays)
  {
    if (overlay.first->isVirtual() && overlay.first->isVirtualVisible())
    {
      overlay.first->setVisible(true);
      overlay.first->renderToTexture();
      overlay.first->setVisible(false);
    }
  }
}

void OverlayCompositor::onVirtualOverlayToggled(const QPointer<VirtualOverlay>& overlay, bool enabled)
{
  assert_return(m_overlays.count(overlay) > 0);
  setChildValue(overlay->getRootNode(), enabled && overlay->isVirtualVisible());
}

void OverlayCompositor::onVirtualOverlayVisibilityToggled(const QPointer<VirtualOverlay>& overlay, bool enabled)
{
  assert_return(m_overlays.count(overlay) > 0);
  setChildValue(overlay->getRootNode(), enabled && overlay->isVirtual());
}

void OverlayCompositor::onVirtualOverlayDestroyed(QObject* object)
{
  for (auto it = m_overlays.begin(); it != m_overlays.end(); ++it)
  {
    if (it->first.data() == object)
    {
      removeChild(it->second.rootNode);
      m_overlays.erase(it);
      return;
    }
  }

  OSGH_LOG_WARN("OverlayCompositor: tried to remove virtual overlay that does not exist");
  assert(false);
}

}
