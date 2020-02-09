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
  return (length / vec.Length()) * vec;
}

b2Vec2 normalized(b2Vec2 vec) {
  vec.Normalize();
  return vec;
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
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    
    const b2Vec2 toTarget = normalized(targetPos - shipPos);
    const b2Vec2 relativeVel = targetVel - shipVel;
    const b2Vec2 desiredPos = targetPos - behave.dist * toTarget + 0.5f * relativeVel;
    const b2Vec2 desiredVel = scaleToLength(desiredPos - shipPos, behave.speed);
    const b2Vec2 accel = desiredVel - shipVel;
    const float forwardAccel = b2Dot(accel, toTarget);
    
    const float aimAngle = std::atan2(toTarget.y, toTarget.x);
    rotateByAngle(move, aimAngle - phys.body->GetAngle());
    
    if (b2Abs(forwardAccel) < 0.5f) {
      move.forward = move.reverse = false;
    } else if (forwardAccel < 0.0f) {
      move.reverse = true;
      move.forward = false;
    } else {
      move.forward = true;
      move.reverse = false;
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
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    
    // TODO: could be improved
    // parametric equation for moving point
    // parametric equation for growing circle
    // find point of intersection
    // fails if missile cannot catch up to target
    
    const float timeToReach = (targetPos - shipPos).Length() / shipVel.Length() * 0.6f;
    const b2Vec2 futureTargetPos = targetPos + timeToReach * targetVel;
    const b2Vec2 desiredVel = scaleToLength(futureTargetPos - shipPos, behave.speed);
    const b2Vec2 accel = desiredVel - shipVel;
    
    const float aimAngle = std::atan2(accel.y, accel.x);
    rotateByAngle(move, aimAngle - phys.body->GetAngle());
    
    move.forward = true;
    move.reverse = false;
  });
}
