#pragma once

#include <QWidget>

#include <osg/MatrixTransform>

#include <memory>

namespace QtOsgBridge
{
  class VirtualOverlay : public QWidget
  {
    Q_OBJECT

  public:
    VirtualOverlay();
    ~VirtualOverlay();

    osg::ref_ptr<osg::Node> getRootNode() const;

    void renderToTexture();

    void setGeometry(const QRect& geometry);
    void setGeometry(int x, int y, int w, int h);

    void setVisible(bool visible) override;
    bool shouldBeVisible() const;

    void setColor(osg::Vec4f color);

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

  };
}
