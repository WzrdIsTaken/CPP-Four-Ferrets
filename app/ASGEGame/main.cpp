/// Project finished - 24/04/2022 - 16:05

#include "ASGEGameLib/Driver.hpp"

//#include "testing/NorbertTestScene.hpp"
//#include "testing/BenTestScene.hpp"
//#include "testing/MatteoTestScene.hpp"

#include "scenes/GameData.hpp"
#include "scenes/MainMenuScene.hpp"

int main(int /*argc*/, char* /*argv*/[])
{
  ASGE::GameSettings game_settings;
  game_settings.game_title    = "Four Ferrets";
  game_settings.window_width  = 1280;
  game_settings.window_height = 720;
  game_settings.mode          = ASGE::GameSettings::WindowMode::BORDERLESS_FULLSCREEN;
  game_settings.fixed_ts      = 30;
  game_settings.fps_limit     = 60;
  game_settings.msaa_level    = 16;
  game_settings.vsync         = ASGE::GameSettings::Vsync::ENABLED;

  bcw::Driver::DriverSettings driver_settings;
  driver_settings.max_threads = 2;

  // bcw::Driver game(game_settings, driver_settings, std::make_shared<NorbertTestScene>());
  // bcw::Driver game(game_settings, driver_settings, std::make_shared<BenTestScene>());
  // bcw::Driver game(game_settings, driver_settings, std::make_shared<MatteoTestScene>());

  bcw::Driver game(game_settings, driver_settings, std::make_shared<MainMenuScene>());
  bcw::Driver::getRenderer()->setClearColour(ASGE::COLOURS::BLACK);

  bcw::Driver::getSceneHandler()->addScene<GameData>();

  game.run();
  return 0;
}

// Group members: Ben, Norbert, James and Matteo
// Honorary mention goes to Joe, who was kindly participating in testing the game
// and record the final gameplay footage. ;)
