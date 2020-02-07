//
//  handle input.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "handle input.hpp"

#include "../comps/input.hpp"
#include <entt/entity/registry.hpp>

namespace {

void set(entt::registry &reg, bool MoveInput::*mem, const bool value) {
  reg.view<MoveInput, KeyInput>().less([=](auto &input) {
    input.*mem = value;
  });
}

bool handleKey(entt::registry &reg, const SDL_Scancode key, const bool press) {
  switch (key) {
    case SDL_SCANCODE_UP:
      set(reg, &MoveInput::forward, press);
      return true;
    case SDL_SCANCODE_LEFT:
      set(reg, &MoveInput::left, press);
      return true;
    case SDL_SCANCODE_RIGHT:
      set(reg, &MoveInput::right, press);
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
