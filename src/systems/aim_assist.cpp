//
//  aim_assist.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "aim_assist.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include "../comps/teams.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../utils/physics.hpp"
#include <entt/entity/registry.hpp>

namespace {

class RayCastCallback final : public b2RayCastCallback {
public:
  RayCastCallback(entt::registry &reg, entt::entity ship)
    : reg{reg} {
    shipTeam = reg.get<Team>(ship);
  }

  float ReportFixture(
    b2Fixture *fixture,
    const b2Vec2 &,
    const b2Vec2 &,
    const float fraction
  ) override {
    const entt::entity e = fromUserData(fixture->GetBody()->GetUserData());
    if (!reg.valid(e)) return -1.0f;
    if (reg.get<Team>(e) == shipTeam) return -1.0f;
    target = e;
    return fraction;
  }
  
  entt::entity getTarget() const {
    return target;
  }

private:
  entt::registry &reg;
  Team shipTeam;
  entt::entity target = entt::null;
};

}

float assistAim(entt::registry &reg, entt::entity ship) {
  const float lookDist = 60.0f;
  
  RayCastCallback callback{reg, ship};
  b2Body *body = reg.get<Physics>(ship).body;
  const b2Vec2 shipPos = body->GetPosition();
  const float shipAngle = body->GetAngle();
  
  const b2Vec2 end = shipPos + angleMag(shipAngle, lookDist);
  reg.ctx<b2World>().RayCast(&callback, shipPos, end);
  entt::entity target = callback.getTarget();
  if (target == entt::null) return shipAngle;
  
  b2Body *targetBody = reg.get<Physics>(target).body;
  const b2Vec2 targetPos = targetBody->GetPosition();
  const b2Vec2 targetVel = targetBody->GetLinearVelocity();
  
  const float boltSpeed = reg.get<BlasterParams>(ship).speed;
  const float timeToReach = (shipPos - targetPos).Length() / boltSpeed * 1.5f;
  const b2Vec2 futureTargetPos = targetPos + timeToReach * targetVel;
  
  const b2Vec2 toFutureTarget = futureTargetPos - shipPos;
  return std::atan2(toFutureTarget.y, toFutureTarget.x);
}
