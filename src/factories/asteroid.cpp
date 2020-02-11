//
//  asteroid.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 11/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "asteroid.hpp"

#include "physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeAsteroid(entt::registry &reg) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makeAsteroid(reg.ctx<b2World>(), e));
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{129, 76, 38});
  return e;
}
