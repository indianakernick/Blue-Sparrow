//
//  expire.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "expire.hpp"

#include "timers.hpp"
#include <SDL2/SDL_timer.h>
#include <entt/entity/registry.hpp>

void expireTemporary(entt::registry &reg) {
  reg.view<ExpireTimer>().each([&](entt::entity e, auto timer) {
    if (SDL_TICKS_PASSED(SDL_GetTicks(), timer.done)) {
      reg.destroy(e);
    }
  });
}
