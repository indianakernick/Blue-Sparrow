//
//  find_target.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "find_target.hpp"

#include <box2d/b2_body.h>
#include "../utils/each.hpp"
#include "../comps/teams.hpp"
#include "../comps/arena.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

namespace {

bool outOfRange(const float ratio, const b2Vec2 diff, const float axisDist) {
  // The viewing rectangle of AIs depends on the viewing rectangle of the player
  if (ratio > 1.0f) {
    return std::abs(diff.x) > axisDist || std::abs(diff.y) > axisDist / ratio;
  } else {
    return std::abs(diff.y) > axisDist || std::abs(diff.x) > axisDist * ratio;
  }
}

bool outOfRange(
  entt::registry &reg,
  const entt::entity target,
  const b2Vec2 fromPos,
  const float axisDist
) {
  auto cam = reg.ctx<Camera>();
  const float ratio = static_cast<float>(cam.width) / cam.height;
  const b2Vec2 toPos = reg.get<Physics>(target).body->GetPosition();
  const b2Vec2 diff = fromPos - toPos;
  return outOfRange(ratio, diff, axisDist);
}

template <typename Func>
entt::entity nearest(
  entt::registry &reg,
  const b2Vec2 fromPos,
  const Team fromTeam,
  const float axisDist,
  Func valid
) {
  // TODO: Use Box2D to query AABB
  entt::entity nearest = entt::null;
  float nearestDist = INFINITY;
  auto cam = reg.ctx<Camera>();
  const float ratio = static_cast<float>(cam.width) / cam.height;
  entt::each(reg, [&](entt::entity e, Physics phys) {
    if (!valid(reg, e, fromTeam)) return;
    
    const b2Vec2 toPos = phys.body->GetPosition();
    const b2Vec2 diff = fromPos - toPos;
    if (outOfRange(ratio, diff, axisDist)) return;
    const float euclidDist = diff.x * diff.x + diff.y * diff.y;
    if (euclidDist > nearestDist) return;
    
    nearest = e;
    nearestDist = euclidDist;
  });
  return nearest;
}

bool isEnemyShip(entt::registry &reg, const entt::entity e, const Team fromTeam) {
  if (!reg.has<Type>(e)) return false;
  if (reg.get<Type>(e) != Type::ship) return false;
  if (!reg.has<Team>(e)) return false;
  if (reg.get<Team>(e) == fromTeam) return false;
  return true;
}

bool isBeacon(entt::registry &reg, const entt::entity e, const Team fromTeam) {
  if (auto *beacon = reg.try_get<Beacon>(e)) {
    switch (beacon->state) {
      case BeaconState::ally:
        return fromTeam == Team::enemy;
      case BeaconState::enemy:
        return fromTeam == Team::ally;
      case BeaconState::neutral:
        return true;
    }
  }
  return false;
}

}

void findEnemyShipTarget(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Team team, ViewDistance dist, TargetEnemyShip params, Target &target) {
    const b2Vec2 pos = phys.body->GetPosition();
    if (params.eager || !reg.valid(target.e) || outOfRange(reg, target.e, pos, dist.max)) {
      target.e = nearest(reg, pos, team, dist.max, isEnemyShip);
    }
  });
}

void findBeaconTarget(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Team team, ViewDistance dist, TargetBeacon, Target &target) {
    const b2Vec2 pos = phys.body->GetPosition();
    target.e = nearest(reg, pos, team, dist.max, isBeacon);
  });
}
