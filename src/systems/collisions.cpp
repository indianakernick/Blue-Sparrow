//
//  collisions.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "collisions.hpp"

#include "arena.hpp"
#include "destruction.hpp"
#include "../comps/ammo.hpp"
#include "../comps/teams.hpp"
#include "../comps/arena.hpp"
#include "../comps/params.hpp"
#include "../comps/collisions.hpp"
#include <entt/entity/registry.hpp>

namespace {

template <typename Func>
bool visit(entt::registry &reg, CollisionPair pair, Func func) {
  if (func(reg, pair.a, pair.b)) return true;
  return func(reg, pair.b, pair.a);
}

bool dealDamage(entt::registry &reg, const entt::entity a, const entt::entity b) {
  if (auto *damage = reg.try_get<Damage>(a)) {
    if (auto *hull = reg.try_get<Hull>(b)) {
      hull->h -= damage->d;
      if (hull->h <= 0) {
        destroyShip(reg, b, damage->killer);
      }
    } else if (auto *beacon = reg.try_get<Beacon>(b)) {
      damageBeacon(*beacon, reg.get<Team>(a), damage->d);
    }
    
    reg.destroy(a);
    return true;
  }
  return false;
}

}

void handleCollisions(entt::registry &reg) {
  auto &collisions = reg.ctx<CollisionPairs>();
  for (CollisionPair pair : collisions) {
    if (!reg.valid(pair.a) || !reg.valid(pair.b)) {
      continue;
    }
    if (visit(reg, pair, dealDamage)) continue;
  }
  collisions.clear();
}

void handlePostCollisions(entt::registry &reg) {
  auto &collisions = reg.ctx<PostCollisionPairs>();
  for (PostCollisionPair pair : collisions) {
    if (!reg.valid(pair.a) || !reg.valid(pair.b)) {
      continue;
    }
    
    if (reg.has<Hull>(pair.a) && reg.has<Hull>(pair.b)) {
      collideShipPair(reg, pair.a, pair.b, pair.impulse);
    } else if (reg.has<Hull>(pair.a)) {
      collideShip(reg, pair.a, pair.impulse);
    } else if (reg.has<Hull>(pair.b)) {
      collideShip(reg, pair.b, pair.impulse);
    }
  }
  collisions.clear();
}
