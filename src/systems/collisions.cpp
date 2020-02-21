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
#include <entt/entity/registry.hpp>

namespace {

using CollisionPair = std::pair<entt::entity, entt::entity>;
using Collisions = std::vector<CollisionPair>;

template <typename Func>
bool visit(CollisionPair pair, Func func) {
  if (func(pair.first, pair.second)) return true;
  return func(pair.second, pair.first);
}

}

void handleCollisions(entt::registry &reg) {
  Collisions &collisions = reg.ctx<Collisions>();
  for (CollisionPair pair : collisions) {
    if (!reg.valid(pair.first) || !reg.valid(pair.second)) {
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
    
    if (reg.has<Hull>(pair.first) && reg.has<Hull>(pair.second)) {
      collideShipPair(reg, pair.first, pair.second);
      continue;
    }
    
    handled = visit(pair, [&reg](entt::entity a, entt::entity b) {
      if (reg.has<Hull>(a)) {
        collideShip(reg, a, b);
        return true;
      }
      return false;
    });
    if (handled) continue;
  }
  collisions.clear();
}
