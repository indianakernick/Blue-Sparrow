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
#include "../comps/ammo.hpp"
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

namespace {

const float minSpeed = 4.0f;
const float damagePerSpeed = 0.5f;

float relativeSpeed(entt::registry &reg, const entt::entity a, const entt::entity b) {
  // TODO: This could be improved
  // http://www.iforce2d.net/b2dtut/collision-anatomy
  const b2Vec2 velA = reg.get<Physics>(a).body->GetLinearVelocity();
  const b2Vec2 velB = reg.get<Physics>(b).body->GetLinearVelocity();
  return (velA - velB).Length();
}

}

void collideShipPair(entt::registry &reg, const entt::entity a, const entt::entity b) {
  const float speed = relativeSpeed(reg, a, b);
  const int damage = (speed - minSpeed) * damagePerSpeed + 0.5f;
  int &hullA = reg.get<Hull>(a).h;
  int &hullB = reg.get<Hull>(b).h;
  // TODO: Should both ships take the same amount of damage?
  // Maybe hull params could store the "toughness" as minSpeed and damagePerSpeed
  hullA -= damage;
  hullB -= damage;
  // This function exists to handle the case where two ships destroy each other
  if (hullA < 0) {
    destroyShip(reg, a);
  }
  if (hullB < 0) {
    destroyShip(reg, b);
  }
}

void collideShip(entt::registry &reg, const entt::entity a, const entt::entity b) {
  const float speed = relativeSpeed(reg, a, b);
  const int damage = std::max(0.0f, (speed - minSpeed) * damagePerSpeed);
  int &hull = reg.get<Hull>(a).h;
  hull -= damage;
  if (hull < 0) {
    destroyShip(reg, a);
  }
}
