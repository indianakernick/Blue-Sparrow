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
#include "../comps/drops.hpp"
#include "../comps/params.hpp"
#include "../comps/collisions.hpp"
#include <entt/entity/registry.hpp>

namespace {

template <typename Func>
bool visit(CollisionPair pair, Func func) {
  if (func(pair.a, pair.b)) return true;
  return func(pair.b, pair.a);
}

}

void handleCollisions(entt::registry &reg) {
  auto &collisions = reg.ctx<CollisionPairs>();
  for (CollisionPair pair : collisions) {
    if (!reg.valid(pair.a) || !reg.valid(pair.b)) {
      continue;
    }
    
    bool handled = false;
    
    handled = visit(pair, [&reg](entt::entity a, entt::entity b) {
      if (auto *damage = reg.try_get<Damage>(a)) {
        if (auto *hull = reg.try_get<Hull>(b)) {
          hull->h -= damage->d;
          if (hull->h <= 0) {
            destroyShip(reg, b);
          }
        } else if (auto *beacon = reg.try_get<Beacon>(b)) {
          damageBeacon(*beacon, reg.get<Team>(a), damage->d);
        }
        
        reg.destroy(a);
        return true;
      }
      return false;
    });
    if (handled) continue;
    
    handled = visit(pair, [&reg](entt::entity a, entt::entity b) {
      if (auto *coins = reg.try_get<Coins>(a)) {
        if (reg.has<Coin>(b)) {
          reg.destroy(b);
          ++coins->c;
          return true;
        }
      }
      return false;
    });
    if (handled) continue;
    
    handled = visit(pair, [&reg](entt::entity a, entt::entity b) {
      if (auto *ammo = reg.try_get<MissileAmmo>(a)) {
        if (reg.has<Ammo>(b)) {
          reg.destroy(b);
          ++ammo->n;
          return true;
        }
      }
      return false;
    });
    if (handled) continue;
    
    handled = visit(pair, [&reg](entt::entity a, entt::entity b) {
      if (auto *hull = reg.try_get<Hull>(a)) {
        if (reg.has<Scrap>(b)) {
          if (hull->h < reg.get<HullParams>(a).durability) {
            reg.destroy(b);
            ++hull->h;
          }
          return true;
        }
      }
      return false;
    });
    if (handled) continue;
  }
  collisions.clear();
}

void handlePostCollisions(entt::registry &reg) {
  auto &collisions = reg.ctx<PostCollisionPairs>();
  for (PostCollisionPair pair : collisions) {
    if (!reg.valid(pair.a) && !reg.valid(pair.b)) {
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
