//
//  collisions.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "collisions.hpp"

#include "../comps/ammo.hpp"
#include <entt/entity/registry.hpp>

namespace {

using CollisionPair = std::pair<entt::entity, entt::entity>;
using Collisions = std::vector<CollisionPair>;

template <typename Func>
void visit(CollisionPair pair, Func func) {
  if (!func(pair.first, pair.second)) {
    func(pair.second, pair.first);
  }
}

}

void handleCollisions(entt::registry &reg) {
  Collisions &collisions = reg.ctx<Collisions>();
  for (CollisionPair pair : collisions) {
    if (!reg.valid(pair.first) || !reg.valid(pair.second)) {
      continue;
    }
    
    visit(pair, [&reg](entt::entity a, entt::entity b) {
      if (auto *hull = reg.try_get<Hull>(a)) {
        if (auto *damage = reg.try_get<Damage>(b)) {
          hull->h -= damage->d;
          reg.destroy(b);
          if (hull->h <= 0) {
            reg.destroy(a);
          }
          return true;
        }
      }
      return false;
    });
  }
}
