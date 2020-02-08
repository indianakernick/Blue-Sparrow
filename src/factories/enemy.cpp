//
//  enemy.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "enemy.hpp"

#include "physics.hpp"
#include "../comps/teams.hpp"
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeEnemy(entt::registry &reg) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makeSmallShip(reg.ctx<b2World>(), Team::enemy));
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{191, 63, 0});
  reg.assign<OrbitBehaviour>(e, 30.0f);
  reg.assign<Target>(e);
  
  MoveParams moveParams;
  moveParams.forwardForce = 60.0f;
  moveParams.reverseForce = 30.0f;
  moveParams.turnTorque = 200.0f;
  reg.assign<MoveParams>(e, moveParams);
  reg.assign<MoveInput>(e);
  
  BlasterParams blasterParams;
  blasterParams.rof = 1.5f;
  blasterParams.speed = 40.0f;
  reg.assign<BlasterParams>(e, blasterParams);
  reg.assign<BlasterInput>(e);
  reg.assign<BlasterTimer>(e, std::uint32_t{});
  
  reg.assign<Team>(e, Team::enemy);
  reg.assign<Type>(e, Type::ship);
  return e;
}
