#pragma once

#include <QWidget>

#include <osg/MatrixTransform>

#include <memory>

namespace QtOsgBridge
{
  class VirtualOverlay : public QWidget
  {
    Q_OBJECT
    Q_PROPERTY(QRect geometry READ geometry WRITE setGeometry)
    Q_PROPERTY(QPoint position READ position WRITE setPosition)

  public:
    VirtualOverlay();
    ~VirtualOverlay() override;

    osg::ref_ptr<osg::Node> getRootNode() const;

    void renderToTexture();

    void setGeometry(const QRect& geometry);
    void setGeometry(int x, int y, int w, int h);

    void setPosition(const QPoint& position);
    QPoint position() const;

    void setColor(osg::Vec4f color);

    void setVirtual(bool enabled);
    bool isVirtual() const;

    void setVirtualVisible(bool visible);
    bool isVirtualVisible() const;

  Q_SIGNALS:
    void toggledIsVirtual(const QPointer<VirtualOverlay>& overlay, bool enabled);
    void toggledIsVirtualVisible(const QPointer<VirtualOverlay>& overlay, bool enabled);

  private:
    struct Impl;
    std::unique_ptr<Impl> m;

  };
}
