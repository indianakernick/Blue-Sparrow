//
//  weapons.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "weapons.hpp"

#include "physics.hpp"
#include "../comps/ai.hpp"
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeBolt(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  setSmallBoltPhysics(reg, e, team);
  reg.emplace<SpriteRect>(e);
  reg.emplace<Sprite>(e, Sprite{255, 0, 0});
  reg.emplace<ExpireTimer>(e, reg.ctx<Now>().time + 5000);
  reg.emplace<Team>(e, team);
  reg.emplace<Type>(e, Type::bullet);
  return e;
}

entt::entity makeMissile(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  setSmallMissilePhysics(reg, e, team);
  reg.emplace<MissileAI>(e);
  reg.emplace<SpriteRect>(e);
  reg.emplace<Sprite>(e, Sprite{191, 191, 191});
  reg.emplace<ExpireTimer>(e, reg.ctx<Now>().time + 15000);
  reg.emplace<Target>(e);
  reg.emplace<MotionCommand>(e);
  reg.emplace<Team>(e, team);
  reg.emplace<Type>(e, Type::bullet);
  return e;
}
