//
// Created by Norbert on 23/03/2022.
//

#include "../../../../include/ASGEGameLib/components/map/TileSquare.hpp"
#include "../../../../include/ASGEGameLib/components/collision/Collider.hpp"

using driver = bcw::Driver;

TileSquare::TileSquare(const tmx::Tileset::Tile* tile, int mag, int x, int y, int16_t z) :
  visibility(true), state(PASSABLE)
{
  sprite = driver::getRenderer()->createUniqueSprite();
  if (sprite->loadTexture(tile->imagePath))
  {
    // The rectangle will use [0] and [1] for the beginning x and y
    // position in the texture and [2] and [3] for the width and height.
    sprite->srcRect()[0] = static_cast<float>(tile->imagePosition.x);
    sprite->srcRect()[1] = static_cast<float>(tile->imagePosition.y);
    sprite->srcRect()[2] = static_cast<float>(tile->imageSize.x);
    sprite->srcRect()[3] = static_cast<float>(tile->imageSize.y);

    sprite->width(static_cast<float>(tile->imageSize.x));
    sprite->height(static_cast<float>(tile->imageSize.y));

    sprite->scale(static_cast<float>(mag));
    sprite->setMagFilter(ASGE::Texture2D::MagFilter::NEAREST);

    sprite->yPos(static_cast<float>(y));
    sprite->xPos(static_cast<float>(x));

    sprite->setGlobalZOrder(z);
  }
}

TileSquare::TileSquare(bool is_visible) : sprite(nullptr), visibility(is_visible), state(PASSABLE)
{
}

void TileSquare::renderTile()
{
  if (visibility)
  {
    driver::getRenderer()->render(*sprite);
  }
}

bool TileSquare::isPassable()
{
  return state == PASSABLE;
}

void TileSquare::setToBlocking()
{
  state = BLOCKING;
}

void TileSquare::setToDestroyable()
{
  state = DESTROYABLE;
}

void TileSquare::collision()
{
  //  if (state == DESTROYABLE)
  //  {
  //    visibility = false;
  //    state      = PASSABLE;
  //  }
}

void TileSquare::addCollider()
{
  collider = std::make_unique<bcw::Collider>(
    [this](bcw::Collider* collision) { onCollisionEnter(collision); },
    [this](bcw::Collider* collision) { onCollisionExit(collision); },
    sprite.get(),
    nullptr);
  collider->setBoolWall(true);
}

double TileSquare::getDistance(ASGE::Point2D from_location)
{
  double a = (from_location.x - sprite->midpoint().x) * (from_location.x - sprite->midpoint().x);
  double b = (from_location.y - sprite->midpoint().y) * (from_location.y - sprite->midpoint().y);
  return sqrt(a + b);
}

ASGE::SpriteBounds TileSquare::getBounds()
{
  return sprite->getWorldBounds();
}

bool TileSquare::checkCollision(ASGE::SpriteBounds obj_bounds)
{
  return (
  obj_bounds.v2.x >= sprite->getWorldBounds().v1.x &&
  obj_bounds.v1.x <= sprite->getWorldBounds().v2.x &&
  obj_bounds.v3.y >= sprite->getWorldBounds().v1.y &&
  obj_bounds.v2.y <= sprite->getWorldBounds().v3.y
  );
}
