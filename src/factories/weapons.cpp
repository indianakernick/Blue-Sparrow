//
//  weapons.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "weapons.hpp"

#include "physics.hpp"
#include <SDL2/SDL_timer.h>
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeBolt(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  setSmallBoltPhysics(reg, e, team);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{255, 0, 0});
  reg.assign<ExpireTimer>(e, SDL_GetTicks() + 50000);
  reg.assign<Team>(e, team);
  reg.assign<Type>(e, Type::bullet);
  return e;
}

entt::entity makeMissile(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  setSmallMissilePhysics(reg, e, team);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{191, 191, 191});
  reg.assign<ExpireTimer>(e, SDL_GetTicks() + 10000);
  reg.assign<Target>(e, false);
  reg.assign<MotionCommand>(e);
  reg.assign<Team>(e, team);
  reg.assign<Type>(e, Type::bullet);
  return e;
}
