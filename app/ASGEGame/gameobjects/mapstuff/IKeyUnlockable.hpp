// Created by Ben on 01/04/2022

#pragma once

// interface with data B)

class IKeyUnlockable
{
 public:
  virtual ~IKeyUnlockable() = default;

  enum class KeyColour
  {
    RED,
    BLUE,
    GREEN,
    YELLOW
  };
  KeyColour key_colour = KeyColour::RED;
};
