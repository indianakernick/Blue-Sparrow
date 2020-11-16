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
#include "../utils/each.hpp"
#include <entt/entity/registry.hpp>

void expireTemporary(entt::registry &reg) {
  // TODO: Could sort timers
  const std::uint32_t now = SDL_GetTicks();
  entt::each(reg, [&](entt::entity e, ExpireTimer timer) {
    if (SDL_TICKS_PASSED(now, timer.done)) {
      reg.destroy(e);
    }
  });
}
