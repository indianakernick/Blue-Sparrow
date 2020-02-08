//
//  missile.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "missile.hpp"

#include "physics.hpp"
#include <box2d/b2_body.h>
#include <SDL2/SDL_timer.h>
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeMissile(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makeSmallMissile(reg.ctx<b2World>(), team));
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{191, 191, 191});
  reg.assign<ExpireTimer>(e, SDL_GetTicks() + 20000);
  reg.assign<SeekBehaviour>(e);
  reg.assign<Target>(e);
  reg.assign<MoveInput>(e);
  reg.assign<Team>(e, team);
  reg.assign<Type>(e, Type::bullet);
  return e;
}
