//
//  destruction.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 12/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "destruction.hpp"

#include <random>
#include "../comps/ammo.hpp"
#include "../comps/params.hpp"
#include "../factories/arena.hpp"
#include <entt/entity/registry.hpp>

void destroyShip(entt::registry &reg, const entt::entity ship, entt::entity killer) {
  static std::mt19937 gen;
  
  if (reg.valid(killer)) {
    if (auto *drops = reg.try_get<Drops>(ship)) {
      if (auto *coins = reg.try_get<Coins>(killer)) {
        coins->c += std::uniform_int_distribution{drops->minCoins, drops->maxCoins}(gen);
      }
      if (auto *ammo = reg.try_get<MissileAmmo>(killer)) {
        ammo->n += std::uniform_int_distribution{drops->minAmmo, drops->maxAmmo}(gen);
      }
      if (auto *hull = reg.try_get<Hull>(killer)) {
        hull->h += std::uniform_int_distribution{drops->minScrap, drops->maxScrap}(gen);
        hull->h = std::min(hull->h, reg.get<HullParams>(killer).durability);
      }
    }
  }
  
  reg.destroy(ship);
}

namespace {

constexpr float minImpulse = 100.0f;
constexpr float damagePerImpulse = 1.0f / 10.0f;

int calcDamage(const float impulse) {
  return std::max(0.0f, (impulse - minImpulse) * damagePerImpulse + 0.5f);
}

}

void collideShipPair(
  entt::registry &reg,
  const entt::entity a,
  const entt::entity b,
  const float impulse
) {
  const int damage = calcDamage(impulse);
  int &hullA = reg.get<Hull>(a).h;
  int &hullB = reg.get<Hull>(b).h;
  // TODO: Should both ships take the same amount of damage?
  // Maybe hull params could store the "toughness" as minImpulse and damagePerImpulse
  hullA -= damage;
  hullB -= damage;
  // This function exists to handle the case where two ships destroy each other
  if (hullA <= 0) {
    destroyShip(reg, a, hullB > 0 ? b : entt::null);
  }
  if (hullB <= 0) {
    destroyShip(reg, b, hullA > 0 ? a : entt::null);
  }
}

void collideShip(entt::registry &reg, const entt::entity a, const float impulse) {
  const int damage = calcDamage(impulse);
  int &hull = reg.get<Hull>(a).h;
  hull -= damage;
  if (hull <= 0) {
    destroyShip(reg, a, entt::null);
  }
}
