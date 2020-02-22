//
//  handle_input.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "handle_input.hpp"

#include <SDL2/SDL_mouse.h>
#include "../utils/each.hpp"
#include "../comps/input.hpp"
#include <entt/entity/registry.hpp>

namespace {

template <typename Command>
void setKey(entt::registry &reg, bool Command::*mem, const bool value) {
  entt::each(reg, [=](Command &command, KeyInput) {
    command.*mem = value;
  });
}

template <typename Command>
void setMouse(entt::registry &reg, bool Command::*mem, const bool value) {
  entt::each(reg, [=](Command &command, MouseInput) {
    command.*mem = value;
  });
}

bool handleKey(entt::registry &reg, const SDL_Scancode key, const bool press) {
  switch (key) {
    case SDL_SCANCODE_E:
      setKey(reg, &MotionCommand::forward, press);
      return true;
    case SDL_SCANCODE_D:
      setKey(reg, &MotionCommand::reverse, press);
      return true;
    case SDL_SCANCODE_S:
      setKey(reg, &MotionCommand::left, press);
      return true;
    case SDL_SCANCODE_F:
      setKey(reg, &MotionCommand::right, press);
      return true;
    
    default:
      return false;
  }
}

}

bool handleKeyDown(entt::registry &reg, const SDL_Scancode key) {
  return handleKey(reg, key, true);
}

bool handleKeyUp(entt::registry &reg, const SDL_Scancode key) {
  return handleKey(reg, key, false);
}

bool handleMouseMove(entt::registry &reg, const int x, const int y) {
  entt::each(reg, [&](MouseInput &mouse) {
    mouse.x = x;
    mouse.y = y;
  });
  return true;
}

bool handleMouseDown(entt::registry &reg, const int button) {
  if (button == SDL_BUTTON_LEFT) {
    setMouse(reg, &BlasterCommand::fire, true);
    return true;
  } else if (button == SDL_BUTTON_RIGHT) {
    setMouse(reg, &MissileCommand::fire, true);
    return true;
  } else {
    return false;
  }
}

bool handleMouseUp(entt::registry &reg, const int button) {
  if (button == SDL_BUTTON_LEFT) {
    setMouse(reg, &BlasterCommand::fire, false);
    return true;
  } else if (button == SDL_BUTTON_RIGHT) {
    setMouse(reg, &MissileCommand::fire, false);
    return true;
  } else {
    return false;
  }
}
