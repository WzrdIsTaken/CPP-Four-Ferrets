//
// Created by Norbert on 03/03/2022.
//

#ifndef ASGEGAME_CAMERA_HPP
#define ASGEGAME_CAMERA_HPP

#include <algorithm>

#include "../../Component.hpp"
#include "../../components/camera/HUD.hpp"
#include <Engine/Logger.hpp>

class Camera : bcw::Component
{
 public:
  Camera(float width, float height, ASGE::Point2D focal_point, const ASGE::Font* font_face);
  ~Camera() override = default;

  void update(const ASGE::GameTime& game_time) override;
  void render();

  void moveCamera(ASGE::Point2D vector);
  ASGE::Point2D getCameraPos();
  ASGE::Point2D getCameraSize();
  ASGE::Camera getCamera();
  void setCameraClamp(ASGE::Camera::CameraView clamp_value);
  void setID(uint16_t id);

  [[nodiscard]] HUD* getHud();

 private:
  [[maybe_unused]] uint16_t cam_id;
  ASGE::Camera camera;
  ASGE::Point2D camera_size;
  ASGE::Camera::CameraView clamp;
  HUD hud;

  [[nodiscard]] ASGE::Point2D checkClamp(ASGE::Point2D new_pos) const;
};

#endif // ASGEGAME_CAMERA_HPP
