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
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

namespace {

entt::entity nearestShip(entt::registry &reg, b2Vec2 fromPos, Team fromTeam) {
  entt::entity nearest = entt::null;
  float nearestDist = INFINITY;
  entt::each(reg, [&](entt::entity e, Physics phys, Team team, Type type) {
    if (type != Type::ship) return;
    if (team == fromTeam) return;
    const b2Vec2 toPos = phys.body->GetPosition();
    const b2Vec2 diff = fromPos - toPos;
    const float dist = diff.x * diff.x + diff.y * diff.y;
    if (dist < nearestDist) {
      nearest = e;
      nearestDist = dist;
    }
  });
  return nearest;
}

}

void findTarget(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Team team, Target &target) {
    target.e = nearestShip(reg, phys.body->GetPosition(), team);
  });
}
