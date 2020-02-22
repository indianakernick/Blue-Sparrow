//
//  behave.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "behave.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include "../utils/each.hpp"
#include <box2d/b2_fixture.h>
#include "../comps/teams.hpp"
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

void rotateByAngle(MoveCommand &move, const float deltaAngle, const float threshold = 0.5f) {
  if (b2Abs(deltaAngle) < threshold * deg2rad) {
    move.ccw = move.cw = false;
  } else if (deltaAngle < 0.0f) {
    move.cw = false;
    move.ccw = true;
  } else {
    move.ccw = false;
    move.cw = true;
  }
}

void moveByAccel(MoveCommand &move, const float accel) {
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
  // https://wiki.unity3d.com/index.php/Calculating_Lead_For_Projectiles?_ga=2.202661201.1952245439.1581310561-814066108.1581310561
  
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

// TODO: I wonder if I could use the genetic algorithm to train the ultimate bot!

void behaveOrbit(entt::registry &reg) {
  auto view = reg.view<Physics, Target, MoveCommand, BlasterCommand, OrbitBehaviour, BlasterParams>();
  view.each([&](auto phys, auto target, auto &move, auto &blaster, auto behave, auto params) {
    if (target.e == entt::null) {
      blaster.fire = false;
      move.forward = move.reverse = move.ccw = move.cw = false;
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
    
    b2Vec2 aimPos;
    switch (behave.level) {
      case OrbitLevel::aim_pos:
        aimPos = targetPos;
        break;
      case OrbitLevel::aim_ahead:
        aimPos = interseptPoint(targetPos, targetVel - shipVel, shipPos, params.speed);
        break;
    }
    
    const b2Vec2 toAim = aimPos - shipPos;
    const float aimAngle = std::atan2(toAim.y, toAim.x);
    rotateByAngle(move, normalizeAngle(aimAngle - phys.body->GetAngle()));
  });
}

void behaveSeek(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Target target, MoveCommand &move, SeekBehaviour behave) {
    if (target.e == entt::null) {
      move.forward = move.reverse = move.ccw = move.cw = false;
      return;
    }
    
    if (behave.level == SeekLevel::no_aim) {
      move.forward = true;
      move.reverse = move.ccw = move.cw = false;
      return;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    
    b2Vec2 accel;
    switch (behave.level) {
      case SeekLevel::aim_pos:
        accel = targetPos - shipPos;
        break;
      case SeekLevel::aim_vel_pos: {
        const b2Vec2 desiredVel = scaleToLength(targetPos - shipPos, behave.speed);
        accel = desiredVel - shipVel;
        break;
      }
      case SeekLevel::aim_vel_ahead: {
        const b2Vec2 aimPos = interseptPoint(targetPos, targetVel, shipPos, behave.speed);
        const b2Vec2 desiredVel = scaleToLength(aimPos - shipPos, behave.speed);
        accel = desiredVel - shipVel;
        break;
      }
      case SeekLevel::no_aim: ;
    }
    
    const float aimAngle = std::atan2(accel.y, accel.x);
    const float angleChange = normalizeAngle(aimAngle - phys.body->GetAngle());
    rotateByAngle(move, angleChange);
    
    move.forward = (b2Abs(angleChange) < b2_pi / 4.0f);
    move.reverse = false;
  });
}

void behaveSniper(entt::registry &reg) {
  auto view = reg.view<Physics, Target, MoveCommand, BlasterCommand, SniperBehaviour, BlasterParams>();
  view.less([&](auto phys, auto target, auto &move, auto &blaster, auto params) {
    if (target.e == entt::null) {
      blaster.fire = false;
      move.forward = move.reverse = move.ccw = move.cw = false;
      return;
    } else {
      blaster.fire = true;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    const float shipAngle = phys.body->GetAngle();
    
    const b2Vec2 aimPos = interseptPoint(targetPos, targetVel - shipVel, shipPos, params.speed);
    const b2Vec2 toAim = aimPos - shipPos;
    const float aimAngle = std::atan2(toAim.y, toAim.x);
    rotateByAngle(move, normalizeAngle(aimAngle - shipAngle), 0.1f);
  });
}

namespace {

class QueryCallback final : public b2QueryCallback {
public:
  QueryCallback(entt::registry &reg, const b2Vec2 point)
    : reg{reg}, point{point} {}

  bool ReportFixture(b2Fixture *fix) override {
    entt::entity e = fromUserData(fix->GetBody()->GetUserData());
    if (!reg.valid(e)) return true;
    if (!reg.has<Type>(e)) return true;
    if (reg.get<Type>(e) != Type::ship) return true;
    if (!reg.has<Team>(e)) return true;
    if (reg.get<Team>(e) != Team::enemy) return true;
    if (!fix->TestPoint(point)) return true;
    fixture = fix;
    return false;
  }
  
  b2Fixture *getFixture() const {
    return fixture;
  }
  
private:
  entt::registry &reg;
  b2Vec2 point;
  b2Fixture *fixture = nullptr;
};

}

void behaveMouse(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, Physics phys, MoveCommand &move, MouseInput mouse, BlasterParams params) {
    const auto &cam = reg.ctx<Camera>();
    const b2Vec2 shipPos = phys.body->GetPosition();
    float aimX = mouse.x / cam.zoom + cam.x;
    float aimY = mouse.y / cam.zoom + cam.y;
    
    if (reg.has<AimAssist>(e)) {
      QueryCallback callback{reg, {aimX, aimY}};
      b2AABB aabb;
      aabb.lowerBound = {aimX - 0.001f, aimY - 0.001f};
      aabb.upperBound = {aimX + 0.001f, aimY + 0.001f};
      phys.body->GetWorld()->QueryAABB(&callback, aabb);
      if (b2Fixture *fix = callback.getFixture()) {
        b2Body *body = fix->GetBody();
        const b2Vec2 aim = interseptPoint(
          body->GetPosition(),
          body->GetLinearVelocity() - phys.body->GetLinearVelocity(),
          phys.body->GetPosition(),
          params.speed
        );
        aimX = aim.x;
        aimY = aim.y;
      }
    }
    
    const float aimAngle = std::atan2(
      aimY - shipPos.y,
      aimX - shipPos.x
    );
    rotateByAngle(move, normalizeAngle(aimAngle - phys.body->GetAngle()));
  });
}
