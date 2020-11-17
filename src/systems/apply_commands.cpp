//
//  apply_commands.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "apply_commands.hpp"

#include <random>
#include <box2d/b2_body.h>
#include "../utils/each.hpp"
#include "../comps/ammo.hpp"
#include "../comps/teams.hpp"
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../utils/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include "../factories/weapons.hpp"
#include <entt/entity/registry.hpp>

void applyMotionCommands(entt::registry &reg) {
  entt::each(reg, [](Physics phys, MotionParams params, MotionCommand command) {
    if (command.forward) {
      const b2Vec2 force = angleMag(phys.body->GetAngle(), params.forwardForce);
      phys.body->ApplyForceToCenter(force, true);
    }
    if (command.reverse) {
      const b2Vec2 force = angleMag(phys.body->GetAngle(), -params.reverseForce);
      phys.body->ApplyForceToCenter(force, true);
    }
    if (command.ccw && !command.cw) {
      phys.body->ApplyTorque(-params.turnTorque, true);
    }
    if (command.cw && !command.ccw) {
      phys.body->ApplyTorque(params.turnTorque, true);
    }
    if (command.left && !command.right) {
      const b2Vec2 force = angleMag(phys.body->GetAngle() - b2_pi / 2.0f, params.lateralForce);
      phys.body->ApplyForceToCenter(force, true);
    }
    if (command.right && !command.left) {
      const b2Vec2 force = angleMag(phys.body->GetAngle() + b2_pi / 2.0f, params.lateralForce);
      phys.body->ApplyForceToCenter(force, true);
    }
  });
}

void applyBlasterCommands(entt::registry &reg) {
  static std::mt19937 gen;
  const std::uint32_t now = reg.ctx<Now>().time;
  
  entt::each(reg, [&](entt::entity e, Physics phys, BlasterParams params, BlasterCommand command, BlasterTimer &timer, Team team) {
    if (!command.fire) return;
    
    if (!timePassed(now, timer.done)) return;
    timer.done = now + 1000 / params.rof;
    
    std::uniform_real_distribution<float> dist{-params.spread, params.spread};
    
    const b2Vec2 shipPos = phys.body->GetPosition();
    const float shipAngle = phys.body->GetAngle();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    const float boltAngle = shipAngle + dist(gen);
    const b2Vec2 boltVel = angleMag(boltAngle, params.speed) + shipVel;
    
    entt::entity bolt = makeBolt(reg, team);
    b2Body *boltBody = reg.get<Physics>(bolt).body;
    boltBody->SetTransform(shipPos, shipAngle);
    boltBody->SetLinearVelocity(boltVel);
    phys.body->ApplyLinearImpulseToCenter(-boltBody->GetMass() * boltVel, true);
    reg.emplace<Damage>(bolt, params.damage, e);
  });
}

void applyMissileCommands(entt::registry &reg) {
  const std::uint32_t now = reg.ctx<Now>().time;
  entt::each(reg, [&](entt::entity e, Physics phys, MissileParams params, MissileCommand command, MissileAmmo &ammo, MissileTimer &timer, Team team, ViewDistance dist) {
    if (!command.fire) return;
    if (ammo.n <= 0) return;
    
    if (!timePassed(now, timer.done)) return;
    timer.done = now + 1000 / params.rof;
    --ammo.n;
    
    const b2Vec2 shipPos = phys.body->GetPosition();
    const float shipAngle = phys.body->GetAngle();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    const b2Vec2 missileVel = angleMag(shipAngle, params.speed * 0.25f) + shipVel;
    
    entt::entity missile = makeMissile(reg, team);
    b2Body *missileBody = reg.get<Physics>(missile).body;
    missileBody->SetTransform(shipPos, shipAngle);
    missileBody->SetLinearVelocity(missileVel);
    phys.body->ApplyLinearImpulseToCenter(-missileBody->GetMass() * missileVel, true);
    MotionParams motionParams;
    motionParams.forwardForce = params.forwardForce;
    motionParams.reverseForce = 0.0f;
    motionParams.lateralForce = 0.0f;
    motionParams.turnTorque = params.turnTorque;
    reg.emplace<MotionParams>(missile, motionParams);
    // TODO: Set velocity limit like this everywhere
    reg.emplace<SeekBehaviour>(missile, params.speed * 1.25f, params.level);
    reg.emplace<VelocityLimit>(missile, params.speed);
    reg.emplace<Damage>(missile, params.damage, e);
    reg.emplace<ViewDistance>(missile, dist);
  });
}
