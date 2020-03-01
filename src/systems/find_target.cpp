//
//  find_target.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "find_target.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include "../comps/teams.hpp"
#include "../utils/physics.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

namespace {

class ShipQueryCallback final : public b2QueryCallback {
public:
  ShipQueryCallback(entt::registry &reg, const b2Vec2 center, const Team team)
    : reg{reg}, center{center}, team{team} {}

  bool ReportFixture(b2Fixture *fixture) override {
    const b2Body *body = fixture->GetBody();
    const float dist = b2DistanceSquared(center, body->GetPosition());
    if (dist > shortestDist) return true;
    
    const entt::entity entity = fromUserData(body->GetUserData());
    if (!enemyShip(entity)) return true;
    
    shortestDist = dist;
    nearestEntity = entity;
    
    return true;
  }
  
  entt::entity getNearest() const {
    return nearestEntity;
  }

private:
  entt::registry &reg;
  b2Vec2 center;
  Team team;
  float shortestDist = INFINITY;
  entt::entity nearestEntity = entt::null;
  
  bool enemyShip(const entt::entity entity) const {
    if (!reg.valid(entity)) return false;
    if (!reg.has<Type>(entity)) return false;
    if (reg.get<Type>(entity) != Type::ship) return false;
    if (!reg.has<Team>(entity)) return false;
    if (reg.get<Team>(entity) == team) return false;
    return true;
  }
};

b2AABB viewingRect(entt::registry &reg, const entt::entity e) {
  auto cam = reg.ctx<Camera>();
  const float ratio = static_cast<float>(cam.width) / cam.height;
  const float dist = reg.get<ViewDistance>(e).max;
  b2AABB aabb;
  if (ratio > 1.0f) {
    aabb.lowerBound.x = -dist;
    aabb.upperBound.x = dist;
    aabb.lowerBound.y = -dist / ratio;
    aabb.upperBound.y = dist / ratio;
  } else {
    aabb.lowerBound.x = -dist * ratio;
    aabb.upperBound.x = dist * ratio;
    aabb.lowerBound.y = -dist;
    aabb.upperBound.y = dist;
  }
  return aabb;
}

}

entt::entity findNearestEnemyShip(entt::registry &reg, const entt::entity e) {
  const b2Vec2 pos = reg.get<Physics>(e).body->GetPosition();
  ShipQueryCallback callback{reg, pos, reg.get<Team>(e)};
  b2AABB aabb = viewingRect(reg, e);
  aabb.lowerBound += pos;
  aabb.upperBound += pos;
  reg.ctx<b2World>().QueryAABB(&callback, aabb);
  return callback.getNearest();
}
