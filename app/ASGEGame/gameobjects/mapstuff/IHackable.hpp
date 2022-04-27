// Created by Ben on 16/03/2022

#pragma once

class IHackable
{
 public:
  virtual void onHackBegin(float hack_speed) = 0;
  virtual void onHackEnd()                   = 0;
  virtual bool getHackedState()              = 0;
  virtual ~IHackable()                       = default;
};
