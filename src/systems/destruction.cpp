//
//  destruction.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 12/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "destruction.hpp"

#include <random>
#include <box2d/b2_body.h>
#include "../comps/drops.hpp"
#include "../comps/physics.hpp"
#include "../factories/arena.hpp"
#include <entt/entity/registry.hpp>

void destroyShip(entt::registry &reg, const entt::entity e) {
  static std::mt19937 gen;
  
  std::uniform_real_distribution<float> shiftDist{-0.8f, 0.8f};
  std::uniform_real_distribution<float> angleDist{-b2_pi, b2_pi};
  
  if (auto *drops = reg.try_get<Drops>(e)) {
    const b2Vec2 pos = reg.get<Physics>(e).body->GetPosition();
    std::uniform_int_distribution dist{drops->minCoins, drops->maxCoins};
    for (int coins = dist(gen); coins > 0; --coins) {
      entt::entity coin = makeCoin(reg);
      const b2Vec2 coinPos = {pos.x + shiftDist(gen), pos.y + shiftDist(gen)};
      const float coinAngle = angleDist(gen);
      reg.get<Physics>(coin).body->SetTransform(coinPos, coinAngle);
    }
  }
  
  reg.destroy(e);
}
