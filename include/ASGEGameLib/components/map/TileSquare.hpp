//
// Created by Norbert on 23/03/2022.
//

#ifndef ASGEGAME_TILESQUARE_HPP
#define ASGEGAME_TILESQUARE_HPP

#include "../../Component.hpp"
#include <Engine/OGLGame.hpp>
#include <Engine/Sprite.hpp>
#include <tmxlite/Tileset.hpp>

class TileSquare : public bcw::Component
{
 public:
  TileSquare(const tmx::Tileset::Tile* tile, int mag, int x, int y, int16_t z);
  explicit TileSquare(bool is_visible);
  ~TileSquare() override = default;

  void renderTile();
  bool isPassable();
  void setToBlocking();
  void setToDestroyable();
  void collision();
  ASGE::SpriteBounds getBounds();
  bool checkCollision(ASGE::SpriteBounds obj_bounds);
  void addCollider();
  double getDistance(ASGE::Point2D from_location);

 private:
  void onCollisionEnter(bcw::Collider* /*collision*/) {}
  void onCollisionExit(bcw::Collider* /*collision*/) {}
  std::unique_ptr<ASGE::Sprite> sprite;
  std::unique_ptr<bcw::Collider> collider;

  bool visibility;
  enum State
  {
    PASSABLE    = 0,
    BLOCKING    = 1,
    DESTROYABLE = 2
  };
  [[maybe_unused]] State state;
};

#endif // ASGEGAME_TILESQUARE_HPP
