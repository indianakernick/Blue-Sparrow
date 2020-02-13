//
//  upgrades.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 13/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "upgrades.hpp"

#include "../comps/ammo.hpp"
#include "../comps/params.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

namespace {

// TODO: Remove when apple clang supports this C++20 feature
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"

constexpr HullParams hullParams[] = {
  {.durability = 100},
  {.durability = 125},
  {.durability = 150},
  {.durability = 200}
};

constexpr HullUpgrade hullUpgrades[] = {
  {{.cost = 4, .level = 1}},
  {{.cost = 6, .level = 2}},
  {{.cost = 10, .level = 3}},
  {{.cost = -1, .level = -1}}
};

#pragma clang diagnostic pop

}

int hullUpgradeLevels() {
  return std::size(hullParams);
}

void setHull(entt::registry &reg, const entt::entity e, const int level) {
  reg.assign<Hull>(e, hullParams[level].durability);
  reg.assign<HullParams>(e, hullParams[level]);
  reg.assign<BarRect>(e);
}

void setUpgradableHull(entt::registry &reg, const entt::entity e, const int level) {
  setHull(reg, e, level);
  reg.assign<HullUpgrade>(e, hullUpgrades[level]);
}

bool upgradeHull(entt::registry &reg, const entt::entity e) {
  int &coins = reg.get<Coins>(e).c;
  auto &upgrade = reg.get<HullUpgrade>(e);
  if (upgrade.level == -1) return false;
  if (upgrade.cost > coins) return false;
  coins -= upgrade.cost;
  reg.replace<HullParams>(e, hullParams[upgrade.level]);
  reg.replace<HullUpgrade>(e, hullUpgrades[upgrade.level]);
  return true;
}
