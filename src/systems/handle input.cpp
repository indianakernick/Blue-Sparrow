//
//  handle input.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "handle input.hpp"

#include "../utils/each.hpp"
#include "../comps/input.hpp"
#include <entt/entity/registry.hpp>

namespace {

template <typename Input>
void set(entt::registry &reg, bool Input::*mem, const bool value) {
  entt::each(reg, [=](Input &input, KeyInput) {
    input.*mem = value;
  });
}

bool handleKey(entt::registry &reg, const SDL_Scancode key, const bool press) {
  switch (key) {
    case SDL_SCANCODE_I:
    case SDL_SCANCODE_UP:
      set(reg, &MoveInput::forward, press);
      return true;
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_DOWN:
      set(reg, &MoveInput::reverse, press);
      return true;
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_LEFT:
      set(reg, &MoveInput::left, press);
      return true;
    case SDL_SCANCODE_L:
    case SDL_SCANCODE_RIGHT:
      set(reg, &MoveInput::right, press);
      return true;
    case SDL_SCANCODE_Z:
      set(reg, &BlasterInput::fire, press);
      return true;
    case SDL_SCANCODE_X:
      set(reg, &MissileInput::fire, press);
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
