// Created by Ben on 16/03/2022

#pragma once

class IPickable
{
 public:
  virtual void onPickBegin(float pick_speed) = 0;
  virtual void onPickEnd()                   = 0;
  virtual bool getPickedState()              = 0;

  virtual ~IPickable() = default;
};
