// Created by Ben on 20/01/2022

#include "../../../include/ASGEGameLib/handlers/InputHandler.hpp"

bcw::InputHandler::InputHandler(
  const std::unique_ptr<ASGE::Input>& _asge_input, const bool use_threads) :
  asge_input(_asge_input.get()),
  key_callback_id(_asge_input->addCallbackFnc(ASGE::E_KEY, &InputHandler::keyHandler, this)),
  mouse_callback_id(
    _asge_input->addCallbackFnc(ASGE::E_MOUSE_CLICK, &InputHandler::clickHandler, this))
{
  _asge_input->use_threads = use_threads;
}

bcw::InputHandler::~InputHandler()
{
  asge_input->unregisterCallback(key_callback_id);
  asge_input->unregisterCallback(mouse_callback_id);
}

void bcw::InputHandler::update(const ASGE::GameTime& /*game_time*/)
{
  gamePadHandler();
}

bool bcw::InputHandler::checkClick(const ASGE::ClickEvent* mouse_event, const ASGE::Sprite& sprite)
{
  return checkClick(
    mouse_event,
    bcw::Float4(
      sprite.xPos(),
      sprite.yPos(),
      sprite.width() * sprite.scale(),
      sprite.height() * sprite.scale()));
}

bool bcw::InputHandler::checkClick(const ASGE::ClickEvent* mouse_event, const ASGE::Text& text)
{
  // I have no idea why the text's height needs to be subtracted to the text's y position
  return checkClick(
    mouse_event,
    bcw::Float4(
      text.getPosition().x,
      text.getPosition().y - text.getHeight(),
      text.getWidth(),
      text.getHeight()));
}

bool bcw::InputHandler::checkClick(
  const ASGE::ClickEvent* mouse_event, const bcw::Float4& bounds, const int mouse_button,
  const int mouse_action)
{
  if (mouse_event->button != mouse_button || mouse_event->action != mouse_action)
  {
    return false;
  }

  return mouse_event->xpos > bounds.x && mouse_event->xpos < bounds.x + bounds.width &&
         mouse_event->ypos > bounds.y && mouse_event->ypos < bounds.y + bounds.height;
}

void bcw::InputHandler::keyHandler(const ASGE::SharedEventData data)
{
  const auto* key = dynamic_cast<const ASGE::KeyEvent*>(data.get());
  callObserverFunctionHelper(key_observers, &IKeyHandler::onKey, key);
}

void bcw::InputHandler::clickHandler(const ASGE::SharedEventData data)
{
  const auto* click = dynamic_cast<const ASGE::ClickEvent*>(data.get());
  callObserverFunctionHelper(click_observers, &IClickHandler::onMouse, click);
}

void bcw::InputHandler::gamePadHandler()
{
  for (const auto& gamepad : asge_input->getGamePads())
  {
    callObserverFunctionHelper(game_pad_observers, &IGamePadHandler::onGamePad, gamepad);
  }
}

bool bcw::InputHandler::checkIfControllerHasPriority(ASGE::GamePadData game_pad_input)
{
  // Just exists so that if we ever want more complex handling,
  // then it can just be done in one place.

  return game_pad_input.idx == 0;
}

void bcw::InputHandler::addObservers(const std::shared_ptr<bcw::Scene>& scene)
{
  addObserverHelper(key_observers, scene);
  addObserverHelper(click_observers, scene);
  addObserverHelper(game_pad_observers, scene);
}

void bcw::InputHandler::removeObservers(const std::shared_ptr<bcw::Scene>& scene)
{
  removeObserversHelper(key_observers, scene);
  removeObserversHelper(click_observers, scene);
  removeObserversHelper(game_pad_observers, scene);
}

ASGE::Input* bcw::InputHandler::getInput()
{
  return asge_input;
}
