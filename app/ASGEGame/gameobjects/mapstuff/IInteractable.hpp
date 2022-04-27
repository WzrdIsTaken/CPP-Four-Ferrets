// Created by Ben on 16/03/2022.

#pragma once

class IInteractable
{
 public:
  virtual void onInteractionBegin() = 0;
  virtual void onInteractionEnd()   = 0;
  virtual ~IInteractable()          = default;
};
