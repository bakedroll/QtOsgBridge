#include <QtOsgBridge/OverlayCompositor.h>
#include <QtOsgBridge/VirtualOverlay.h>

#include <QGridLayout>

namespace QtOsgBridge
{

void deleteLayout(QLayout* layout)
{
  QLayoutItem* item      = nullptr;
  QLayout*     sublayout = nullptr;
  QWidget*     widget    = nullptr;

  while ((item = layout->takeAt(0)))
  {
    if ((sublayout = item->layout()) != nullptr)
    {
      deleteLayout(sublayout);
    }
    else if ((widget = item->widget()) != nullptr)
    {
      widget->hide();
      delete widget;
    }
    else
    {
      delete item;
    }
  }

  delete layout;
}

void deleteWidget(QWidget* widget)
{
  if (widget->layout())
  {
    deleteLayout(widget->layout());
  }

  widget->hide();
  delete widget;
}

struct OverlayCompositor::Impl
{
  using OverlaySet = std::set<VirtualOverlay*>;

  Impl()
    : parentWidget(nullptr)
    , firstFrame(GL_TRUE)
  {}

  QWidget*   parentWidget;
  OverlaySet overlays;

  bool firstFrame;
};

OverlayCompositor::OverlayCompositor(QWidget* parent)
  : m(new Impl())
{
  m->parentWidget = parent;
}

OverlayCompositor::~OverlayCompositor() = default;

void OverlayCompositor::addVirtualOverlay(VirtualOverlay* overlay)
{
  if (m->overlays.count(overlay) > 0)
  {
    return;
  }

  addChild(overlay->getRootNode());

  overlay->setParent(m->parentWidget);
  m->overlays.insert(overlay);
}

void OverlayCompositor::removeVirtualOverlay(VirtualOverlay* overlay)
{
  const auto it = m->overlays.find(overlay);
  if (it == m->overlays.end())
  {
    return;
  }

  removeChild(overlay->getRootNode());
  overlay->setParent(nullptr);
  deleteWidget(overlay);

  m->overlays.erase(it);
}

void OverlayCompositor::clear()
{
  for (const auto& overlay : m->overlays)
  {
    removeChild(overlay->getRootNode());

    overlay->setParent(nullptr);
    deleteWidget(overlay);
  }

  m->overlays.clear();
  m->firstFrame = true;
}

void OverlayCompositor::renderVirtualOverlays()
{
  if (m->firstFrame)
  {
    for (const auto& overlay : m->overlays)
    {
      if (overlay->shouldBeVisible())
      {
        overlay->show();
      }
    }

    m->firstFrame = false;
  }

  for (const auto& overlay : m->overlays)
  {
    overlay->renderToTexture();
  }
}

}