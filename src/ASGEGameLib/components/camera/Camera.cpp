//
// Created by Norbert on 03/03/2022.
//

#include "../../../../include/ASGEGameLib/components/camera/Camera.hpp"
Camera::Camera(float width, float height, ASGE::Point2D focal_point, const ASGE::Font* font_face) :
  cam_id(0), camera(ASGE::Camera{ width, height }), camera_size(width, height),
  hud(ASGE::Point2D{ focal_point.x - width * 0.5F, focal_point.y - height * 0.5F }, font_face)
{
  camera.lookAt(focal_point);
}

void Camera::update(const ASGE::GameTime& game_time)
{
  bcw::Driver::getRenderer()->setProjectionMatrix(camera.getView());
  hud.setPosition(ASGE::Point2D{ camera.position().x - camera_size.x * 0.5F,
                                 camera.position().y - camera_size.y * 0.5F });
  hud.update(game_time);
}

void Camera::render()
{
  hud.render();
}

void Camera::moveCamera(ASGE::Point2D vector)
{
  camera.lookAt(checkClamp(ASGE::Point2D{ vector.x, vector.y }));
}

ASGE::Point2D Camera::getCameraPos()
{
  return camera.position();
}

ASGE::Point2D Camera::getCameraSize()
{
  return camera_size;
}

ASGE::Camera Camera::getCamera()
{
  return camera;
}

void Camera::setCameraClamp(ASGE::Camera::CameraView clamp_value)
{
  clamp = clamp_value;
}

ASGE::Point2D Camera::checkClamp(ASGE::Point2D new_pos) const
{
  float new_x = std::round(std::clamp(new_pos.x, clamp.min_x, clamp.max_x));
  float new_y = std::round(std::clamp(new_pos.y, clamp.min_y, clamp.max_y));

  return ASGE::Point2D{ new_x, new_y };
}

void Camera::setID(uint16_t id)
{
  cam_id = id;
  hud.setID(id);
}

HUD* Camera::getHud()
{
  return &hud;
}
