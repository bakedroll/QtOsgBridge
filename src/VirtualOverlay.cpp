#include <QtOsgBridge/VirtualOverlay.h>

#include <osg/Geode>
#include <osg/Material>
#include <osg/Switch>

#include <osgHelper/Helper.h>

#include <cassert>

namespace QtOsgBridge
{

struct VirtualOverlay::Impl
{
  Impl()
    : rootNode(new osg::Switch())
    , transform(new osg::MatrixTransform())
    , geode(new osg::Geode())
    , texture(new osg::Texture2D())
    , material(new osg::Material())
    , bShouldBeVisible(true)
  {
    texture->setDataVariance(osg::Object::DYNAMIC);
    texture->setImage(new osg::Image());
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_BORDER);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_BORDER);
    texture->setFilter(osg::Texture::MIN_FILTER, osg::Texture::NEAREST);
    texture->setFilter(osg::Texture::MAG_FILTER, osg::Texture::NEAREST);
    texture->setResizeNonPowerOfTwoHint(false);

    material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(1.0, 1.0, 1.0, 1.0));

    geode->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
    geode->getOrCreateStateSet()->setAttributeAndModes(material, osg::StateAttribute::ON);

    transform->addChild(geode);
    rootNode->addChild(transform, false);
  }

  void placeAt(int x, int y) const
  {
    auto mat = osg::Matrix::identity();
    mat.setTrans(x, y, 0);

    transform->setMatrix(mat);
  }

  void rescale(int width, int height)
  {
    // resize virtual overlay
    if (geode->getNumDrawables() > 0)
    {
      geode->removeDrawable(geode->getDrawable(0));
    }

    const auto geometry =
            osgHelper::createQuadGeometry(0.0f, static_cast<float>(width) - 1.0f, static_cast<float>(height) - 1.0f,
                                          0.0f, 0.0f, osgHelper::QuadOrientation::XY, false);

    // geode->addDrawable(geometry);

    pixmap = QPixmap(width, height);
    texture->setTextureSize(width, height);
  }

  osg::ref_ptr<osg::Switch>          rootNode;
  osg::ref_ptr<osg::MatrixTransform> transform;
  osg::ref_ptr<osg::Geode>           geode;
  osg::ref_ptr<osg::Texture2D>       texture;
  osg::ref_ptr<osg::Material>        material;

  QPixmap pixmap;
  QImage  image;

  bool bShouldBeVisible;
};

VirtualOverlay::VirtualOverlay()
  : QWidget(nullptr)
  , m(new Impl())
{
  //setAttribute(Qt::WA_DontShowOnScreen);
  //setAttribute(Qt::WA_TranslucentBackground);

  const auto geo = geometry();

  m->placeAt(geo.x(), geo.y());
  m->rescale(geo.width(), geo.height());
}

VirtualOverlay::~VirtualOverlay() = default;

osg::ref_ptr<osg::Node> VirtualOverlay::getRootNode() const
{
  return m->rootNode;
}

void VirtualOverlay::renderToTexture()
{
  const auto width  = m->texture->getTextureWidth();
  const auto height = m->texture->getTextureHeight();

  m->pixmap.fill(QColor(0,0,0,0));
  render(&m->pixmap);

  auto image = m->texture->getImage();
  assert(image);

  m->image = m->pixmap.toImage().convertToFormat(QImage::Format_RGBA8888);
  image->setImage(width, height, 1, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, m->image.bits(), osg::Image::NO_DELETE);
}

void VirtualOverlay::setGeometry(const QRect& geo)
{
  const auto oldGeo = geometry();
  
  // position changes
  if ((geo.x() != oldGeo.x()) || (geo.y() != oldGeo.y()))
  {
    m->placeAt(geo.x(), geo.y());
  }

  // size changes
  if ((geo.width() != oldGeo.width()) || (geo.height() != oldGeo.height()))
  {
    m->rescale(geo.width(), geo.height());
  }

  QWidget::setGeometry(geo);
}

void VirtualOverlay::setGeometry(int x, int y, int w, int h)
{
  setGeometry(QRect(x, y, w, h));
}

void VirtualOverlay::setVisible(bool visible)
{
  m->bShouldBeVisible = visible;
  m->rootNode->setChildValue(m->transform, visible);

  QWidget::setVisible(visible);
}

bool VirtualOverlay::shouldBeVisible() const
{
  return m->bShouldBeVisible;
}

void VirtualOverlay::setColor(osg::Vec4f color)
{
  m->material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
}

}
