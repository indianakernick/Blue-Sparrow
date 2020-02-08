//
//  bolt.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "bolt.hpp"

#include "physics.hpp"
#include <SDL2/SDL_timer.h>
#include "../comps/timers.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeBolt(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makeSmallBullet(reg.ctx<b2World>(), team));
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{255, 0, 0});
  reg.assign<ExpireTimer>(e, SDL_GetTicks() + 10000);
  reg.assign<Team>(e, team);
  reg.assign<Type>(e, Type::bullet);
  return e;
}
