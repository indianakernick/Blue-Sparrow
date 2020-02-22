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

bool outOfRange(entt::registry &reg, entt::entity target, b2Vec2 fromPos, const float axisDist) {
  auto cam = reg.ctx<Camera>();
  const float ratio = static_cast<float>(cam.width) / cam.height;
  const b2Vec2 toPos = reg.get<Physics>(target).body->GetPosition();
  const b2Vec2 diff = fromPos - toPos;
  return outOfRange(ratio, diff, axisDist);
}

entt::entity nearestShip(entt::registry &reg, b2Vec2 fromPos, Team fromTeam, const float axisDist) {
  entt::entity nearest = entt::null;
  float nearestDist = INFINITY;
  auto cam = reg.ctx<Camera>();
  const float ratio = static_cast<float>(cam.width) / cam.height;
  entt::each(reg, [&](entt::entity e, Physics phys, Team team, Type type) {
    if (type != Type::ship) return;
    if (team == fromTeam) return;
    
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

}

void findTarget(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Team team, ViewDistance dist, Target &target) {
    const b2Vec2 pos = phys.body->GetPosition();
    if (target.eager || !reg.valid(target.e) || outOfRange(reg, target.e, pos, dist.max)) {
      target.e = nearestShip(reg, pos, team, dist.max);
    }
  });
}
