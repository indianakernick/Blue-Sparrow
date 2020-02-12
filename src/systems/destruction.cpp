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

namespace {

void setPos(entt::registry &reg, const entt::entity e, const b2Vec2 pos, std::mt19937 &gen) {
  std::uniform_real_distribution<float> shiftDist{-0.8f, 0.8f};
  std::uniform_real_distribution<float> angleDist{-b2_pi, b2_pi};
  std::uniform_real_distribution<float> spinDist{-1.0f, 1.0f};
  const b2Vec2 shiftedPos = {pos.x + shiftDist(gen), pos.y + shiftDist(gen)};
  b2Body *body = reg.get<Physics>(e).body;
  body->SetTransform(shiftedPos, angleDist(gen));
  body->SetAngularVelocity(spinDist(gen));
}

}

void destroyShip(entt::registry &reg, const entt::entity e) {
  static std::mt19937 gen;
  
  if (auto *drops = reg.try_get<Drops>(e)) {
    const b2Vec2 pos = reg.get<Physics>(e).body->GetPosition();
    
    std::uniform_int_distribution coinDist{drops->minCoins, drops->maxCoins};
    for (int coins = coinDist(gen); coins > 0; --coins) {
      setPos(reg, makeCoin(reg), pos, gen);
    }
    
    std::uniform_int_distribution ammoDist{drops->minAmmo, drops->maxAmmo};
    for (int ammo = ammoDist(gen); ammo > 0; --ammo) {
      setPos(reg, makeAmmo(reg), pos, gen);
    }
    
    std::uniform_int_distribution scrapDist{drops->minScrap, drops->maxScrap};
    for (int scrap = scrapDist(gen); scrap > 0; --scrap) {
      setPos(reg, makeScrap(reg), pos, gen);
    }
  }
  
  reg.destroy(e);
}
