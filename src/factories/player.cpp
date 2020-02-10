//
//  player.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "player.hpp"

#include "physics.hpp"
#include "../comps/ammo.hpp"
#include "../comps/teams.hpp"
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makeSmallShip(reg.ctx<b2World>(), Team::ally, e));
  reg.assign<VelocityLimit>(e, 15.0f);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{63, 63, 191});
  reg.assign<CameraFocus>(e);
  reg.assign<KeyInput>(e);
  reg.assign<Hull>(e, 50000); // for testing
  reg.assign<AimAssist>(e);
  
  MoveParams moveParams;
  moveParams.forwardForce = 60.0f;
  moveParams.reverseForce = 30.0f;
  moveParams.turnTorque = 200.0f;
  reg.assign<MoveParams>(e, moveParams);
  reg.assign<MoveInput>(e);
  
  BlasterParams blasterParams;
  blasterParams.rof = 2.0f;
  blasterParams.speed = 60.0f;
  blasterParams.damage = 5;
  reg.assign<BlasterParams>(e, blasterParams);
  reg.assign<BlasterInput>(e);
  reg.assign<BlasterTimer>(e, std::uint32_t{});
  
  MissileParams missileParams;
  missileParams.rof = 0.5f;
  missileParams.speed = 25.0f;
  missileParams.forwardForce = 25.0f;
  missileParams.turnTorque = 3.0f;
  missileParams.damage = 15;
  reg.assign<MissileParams>(e, missileParams);
  reg.assign<MissileInput>(e);
  reg.assign<MissileTimer>(e, std::uint32_t{});
  
  reg.assign<Team>(e, Team::ally);
  reg.assign<Type>(e, Type::ship);
  return e;
}
