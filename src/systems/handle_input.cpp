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

template <typename Input>
void setKey(entt::registry &reg, bool Input::*mem, const bool value) {
  entt::each(reg, [=](Input &input, KeyInput) {
    input.*mem = value;
  });
}

template <typename Input>
void setMouse(entt::registry &reg, bool Input::*mem, const bool value) {
  entt::each(reg, [=](Input &input, MouseInput) {
    input.*mem = value;
  });
}

bool handleKey(entt::registry &reg, const SDL_Scancode key, const bool press) {
  switch (key) {
    case SDL_SCANCODE_E:
      setMouse(reg, &MoveInput::forward, press);
      return true;
    case SDL_SCANCODE_D:
      setMouse(reg, &MoveInput::reverse, press);
      return true;
    case SDL_SCANCODE_I:
    case SDL_SCANCODE_UP:
      setKey(reg, &MoveInput::forward, press);
      return true;
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_DOWN:
      setKey(reg, &MoveInput::reverse, press);
      return true;
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_LEFT:
      setKey(reg, &MoveInput::left, press);
      return true;
    case SDL_SCANCODE_L:
    case SDL_SCANCODE_RIGHT:
      setKey(reg, &MoveInput::right, press);
      return true;
    case SDL_SCANCODE_Z:
      setKey(reg, &BlasterInput::fire, press);
      return true;
    case SDL_SCANCODE_X:
      setKey(reg, &MissileInput::fire, press);
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
    setMouse(reg, &BlasterInput::fire, true);
    return true;
  } else if (button == SDL_BUTTON_RIGHT) {
    setMouse(reg, &MissileInput::fire, true);
    return true;
  } else {
    return false;
  }
}

bool handleMouseUp(entt::registry &reg, const int button) {
  if (button == SDL_BUTTON_LEFT) {
    setMouse(reg, &BlasterInput::fire, false);
    return true;
  } else if (button == SDL_BUTTON_RIGHT) {
    setMouse(reg, &MissileInput::fire, false);
    return true;
  } else {
    return false;
  }
}
