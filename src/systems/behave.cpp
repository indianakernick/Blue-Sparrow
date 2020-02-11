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
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../utils/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

namespace {

float normalizeAngle(float angle) {
  while (angle < -b2_pi) angle += 2.0f * b2_pi;
  while (angle > b2_pi) angle -= 2.0f * b2_pi;
  return angle;
}

void rotateByAngle(MoveInput &move, float deltaAngle) {
  if (b2Abs(deltaAngle) < 0.5f * deg2rad) {
    move.left = move.right = false;
  } else if (deltaAngle < 0.0f) {
    move.right = false;
    move.left = true;
  } else {
    move.left = false;
    move.right = true;
  }
}

void moveByAccel(MoveInput &move, float accel) {
  if (b2Abs(accel) < 0.5f) {
    move.forward = move.reverse = false;
  } else if (accel < 0.0f) {
    move.reverse = true;
    move.forward = false;
  } else {
    move.forward = true;
    move.reverse = false;
  }
}

b2Vec2 scaleToLength(const b2Vec2 vec, const float length) {
  return (length / vec.Length()) * vec;
}

b2Vec2 normalized(b2Vec2 vec) {
  vec.Normalize();
  return vec;
}

b2Vec2 interseptPoint(
  const b2Vec2 targetPos,
  const b2Vec2 targetVel,
  const b2Vec2 launchPos,
  const float bulletSpeed
) {
  // https://gamedev.stackexchange.com/a/25292/72999
  // TODO: Check the determinent
  // This causes a divide by zero if the bullet can't catch up to the target
  
  const b2Vec2 toTarget = targetPos - launchPos;

  const float a = b2Dot(targetVel, targetVel) - bulletSpeed * bulletSpeed;
  const float b = 2.0f * b2Dot(targetVel, toTarget);
  const float c = b2Dot(toTarget, toTarget);

  const float p = -b / (2.0f * a);
  const float q = std::sqrt((b * b) - 4.0f * a * c) / (2.0f * a);

  const float t1 = p - q;
  const float t2 = p + q;
  const float t = (t1 > t2 && t2 > 0.0f ? t2 : t1);

  return targetPos + t * targetVel;
}

}

// TODO: I wonder if I could use the genetic algorithm to train the ultimate bot

void behaveOrbit(entt::registry &reg) {
  auto view = reg.view<Physics, Target, MoveInput, BlasterInput, OrbitBehaviour, BlasterParams>();
  view.each([&](auto phys, auto target, auto &move, auto &blaster, auto behave, auto params) {
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
    moveByAccel(move, b2Dot(accel, toTarget));
    
    /*
    TODO: Maybe put this into a dumber bot
    const float timeToReach = (targetPos - shipPos).Length() / params.speed * 1.5f;
    const b2Vec2 aimPos = targetPos + timeToReach * targetVel;
    */
    
    b2Vec2 aimPos = interseptPoint(targetPos, targetVel, shipPos, params.speed);
    const b2Vec2 toAim = aimPos - shipPos;
    const float aimAngle = std::atan2(toAim.y, toAim.x);
    rotateByAngle(move, normalizeAngle(aimAngle - phys.body->GetAngle()));
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
    
    /*
    const float timeToReach = (targetPos - shipPos).Length() / shipVel.Length() * 0.6f;
    const b2Vec2 aimPos = targetPos + timeToReach * targetVel;
    */
    
    b2Vec2 aimPos = interseptPoint(targetPos, targetVel, shipPos, behave.speed);
    const b2Vec2 desiredVel = scaleToLength(aimPos - shipPos, behave.speed);
    const b2Vec2 accel = desiredVel - shipVel;
    
    const float aimAngle = std::atan2(accel.y, accel.x);
    const float angleChange = normalizeAngle(aimAngle - phys.body->GetAngle());
    rotateByAngle(move, angleChange);
    
    move.forward = (b2Abs(angleChange) < b2_pi / 4.0f);
    move.reverse = false;
  });
}

void behaveMouse(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, MoveInput &move, MouseInput mouse) {
    const auto &cam = reg.ctx<Camera>();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const float aimX = mouse.x / cam.zoom + cam.x;
    const float aimY = mouse.y / cam.zoom + cam.y;
    const float aimAngle = std::atan2(
      aimY - shipPos.y,
      aimX - shipPos.x
    );
    rotateByAngle(move, normalizeAngle(aimAngle - phys.body->GetAngle()));
  });
}
