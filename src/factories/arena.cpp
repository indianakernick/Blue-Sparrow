//
//  arena.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 9/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "arena.hpp"

#include "physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeArena(entt::registry &reg, const float width, const float height) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makeArena(reg.ctx<b2World>(), width, height));
  reg.assign<BackgroundSprite>(e);
  reg.assign<SpriteRect>(e);
  return e;
}
