//
//  behave.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "behave.hpp"

#include <box2d/b2_body.h>
#include "../utils/each.hpp"
#include "../comps/input.hpp"
#include "../comps/physics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

namespace {

void rotateByAngle(MoveInput &move, float deltaAngle) {
  while (deltaAngle < -b2_pi) deltaAngle += 2.0f * b2_pi;
  while (deltaAngle > b2_pi) deltaAngle -= 2.0f * b2_pi;
  
  if (b2Abs(deltaAngle) < 2.0f * b2_pi / 180.0f) {
    move.left = move.right = false;
  } else if (deltaAngle < 0.0f) {
    move.right = false;
    move.left = true;
  } else {
    move.left = false;
    move.right = true;
  }
}

b2Vec2 scaleToLength(const b2Vec2 vec, const float length) {
  return ((length * length) / vec.LengthSquared()) * vec;
}

}

void behaveOrbit(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Target target, MoveInput &move, BlasterInput &blaster, OrbitBehaviour behave) {
    if (target.e == entt::null) {
      blaster.fire = false;
      move.forward = move.reverse = move.left = move.right = false;
      return;
    } else {
      blaster.fire = true;
    }
    
    const b2Vec2 targetPos = reg.get<Physics>(target.e).body->GetPosition();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 toTarget = targetPos - shipPos;
    
    const float aimAngle = std::atan2(toTarget.y, toTarget.x);
    rotateByAngle(move, aimAngle - phys.body->GetAngle());
    
    const float aimDist = behave.dist * behave.dist;
    const float toAimDist = aimDist - (toTarget.x * toTarget.x + toTarget.y * toTarget.y);
    
    if (b2Abs(toAimDist) < 4.0f * 4.0f) {
      move.forward = move.reverse = false;
    } else if (toAimDist < 0.0f) {
      move.forward = true;
      move.reverse = false;
    } else {
      move.reverse = true;
      move.forward = false;
    }
  });
}

void behaveSeek(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Target target, MoveInput &move, SeekBehaviour behave) {
    if (target.e == entt::null) {
      move.forward = move.reverse = move.left = move.right = false;
      return;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    // TODO: account for target velocity
    // const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    const b2Vec2 desiredVel = scaleToLength(targetPos - shipPos, behave.speed);
    const b2Vec2 accel = desiredVel - shipVel;
    
    const float aimAngle = std::atan2(accel.y, accel.x);
    rotateByAngle(move, aimAngle - phys.body->GetAngle());
    
    move.forward = true;
    move.reverse = false;
  });
}
