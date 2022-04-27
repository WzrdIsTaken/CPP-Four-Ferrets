// Created by Ben on 01/02/2022

#include "../../../include/ASGEGameLib/handlers/RenderHandler.hpp"

bcw::RenderHandler::RenderHandler(
  const std::unique_ptr<ASGE::Renderer>& _asge_renderer, const bcw::Vector2Int& base_resolution) :
  asge_renderer(_asge_renderer.get())
{
  asge_renderer->setBaseResolution(
    base_resolution.x, base_resolution.y, ASGE::Resolution::Policy::MAINTAIN);
}

ASGE::Sprite* bcw::RenderHandler::setupSprite(
  ASGE::Sprite* sprite, const std::string& texture, const bcw::Vector2& position, const float scale,
  const short z_order, const ASGE::GameSettings::MagFilter mag_filter)
{
  sprite->loadTexture(texture);
  sprite->scale(scale);
  sprite->setMagFilter(mag_filter);

  sprite->xPos(position.x);
  sprite->yPos(position.y);
  sprite->setGlobalZOrder(z_order);

  return sprite;
}

ASGE::Sprite* bcw::RenderHandler::loadSpriteTextureAndMaintainSettings(
  ASGE::Sprite* sprite, const std::string& texture, const ASGE::GameSettings::MagFilter mag_filter)
{
  // ASGE::Texture2D::MagFilter mag_filter = sprite->getTexture().[):];
  // Doesn't look like you can get the mag filter of a texture, so with this function
  // we will just assume that a NEAREST wants to be used

  // Obviously this isn't all sprite settings... just the ones that caused issues for me xd
  ASGE::Colour old_colour = sprite->colour();

  sprite->loadTexture(texture);
  sprite->setMagFilter(mag_filter);
  sprite->colour(old_colour);

  return sprite;
}

bcw::Vector2 bcw::RenderHandler::getScreenCentre(
  const bcw::RenderHandler::ScreenCentre screen_centre, const ASGE::Text& text,
  const float other_axis_value)
{
  float x = static_cast<float>(asge_renderer->windowWidth() / 2) - text.getWidth() / 2;   // NOLINT
  float y = static_cast<float>(asge_renderer->windowHeight() / 2) - text.getHeight() / 2; // NOLINT

  return actuallyGetScreenCentre(screen_centre, bcw::Vector2(x, y), other_axis_value);
}

bcw::Vector2 bcw::RenderHandler::getScreenCentre(
  const bcw::RenderHandler::ScreenCentre screen_centre, const ASGE::Sprite& sprite,
  const float other_axis_value)
{
  float x = static_cast<float>(asge_renderer->windowWidth() / 2) - sprite.width() / 2;   // NOLINT
  float y = static_cast<float>(asge_renderer->windowHeight() / 2) - sprite.height() / 2; // NOLINT

  return actuallyGetScreenCentre(screen_centre, bcw::Vector2(x, y), other_axis_value);
}

bcw::Vector2 bcw::RenderHandler::actuallyGetScreenCentre(
  const ScreenCentre screen_centre, const bcw::Vector2 bounds, const float other_axis_value)
{
  switch (screen_centre)
  {
    case X:
      return { bounds.x, other_axis_value > INFINITY - 1 ? 0 : other_axis_value };
    case Y:
      return { other_axis_value > INFINITY - 1 ? 0 : other_axis_value, bounds.y };
    case BOTH:
      return { bounds.x, bounds.y };
  }

  // Have to do this INFINITY - 1 because of nice werror when comparing floats with == / !=
  // Also I can't get std::variant to work with passing in into a function ):
}
