//
//  apply_input.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "apply_input.hpp"

#include "aim_assist.hpp"
#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include <SDL2/SDL_timer.h>
#include "../utils/each.hpp"
#include "../comps/ammo.hpp"
#include "../comps/teams.hpp"
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../utils/physics.hpp"
#include "../comps/behaviour.hpp"
#include "../factories/weapons.hpp"
#include <entt/entity/registry.hpp>

void applyMoveInput(entt::registry &reg) {
  entt::each(reg, [](Physics phys, MoveParams params, MoveInput input) {
    if (input.forward) {
      const b2Vec2 force = angleMag(phys.body->GetAngle(), params.forwardForce);
      phys.body->ApplyForceToCenter(force, true);
    }
    if (input.reverse) {
      const b2Vec2 force = angleMag(phys.body->GetAngle(), -params.reverseForce);
      phys.body->ApplyForceToCenter(force, true);
    }
    if (input.left && !input.right) {
      phys.body->ApplyTorque(-params.turnTorque, true);
    }
    if (input.right && !input.left) {
      phys.body->ApplyTorque(params.turnTorque, true);
    }
  });
}

void applyBlasterInput(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, Physics phys, BlasterParams params, BlasterInput input, BlasterTimer &timer, Team team) {
    if (!input.fire) return;
    const std::uint32_t now = SDL_GetTicks();
    if (!SDL_TICKS_PASSED(now, timer.done)) return;
    timer.done = now + 1000 / params.rof;
    
    const b2Vec2 shipPos = phys.body->GetPosition();
    const float shipAngle = phys.body->GetAngle();
    
    entt::entity bolt = makeBolt(reg, team);
    b2Body *boltBody = reg.get<Physics>(bolt).body;
    boltBody->SetTransform(shipPos, shipAngle);
    const float aim = reg.has<AimAssist>(e) ? assistAim(reg, e) : shipAngle;
    boltBody->SetLinearVelocity(angleMag(aim, params.speed));
    reg.assign<Damage>(bolt, params.damage);
  });
}

void applyMissileInput(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, MissileParams params, MissileInput input, MissileTimer &timer, Team team) {
    if (!input.fire) return;
    const std::uint32_t now = SDL_GetTicks();
    if (!SDL_TICKS_PASSED(now, timer.done)) return;
    timer.done = now + 1000 / params.rof;
    
    const b2Vec2 shipPos = phys.body->GetPosition();
    const float shipAngle = phys.body->GetAngle();
    
    entt::entity missile = makeMissile(reg, team);
    b2Body *missileBody = reg.get<Physics>(missile).body;
    missileBody->SetTransform(shipPos, shipAngle);
    missileBody->SetLinearVelocity(angleMag(shipAngle, params.speed * 0.5f));
    MoveParams moveParams;
    moveParams.forwardForce = params.forwardForce;
    moveParams.reverseForce = 0.0f;
    moveParams.turnTorque = params.turnTorque;
    reg.assign<MoveParams>(missile, moveParams);
    reg.assign<SeekBehaviour>(missile, params.speed, params.level);
    reg.assign<VelocityLimit>(missile, params.speed * 0.75f);
    reg.assign<Damage>(missile, params.damage);
  });
}
