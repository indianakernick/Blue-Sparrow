//
//  upgrades.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 13/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "upgrades.hpp"

#include "../comps/ammo.hpp"
#include "../comps/input.hpp"
#include "../comps/params.hpp"
#include "../comps/timers.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

namespace {

// TODO: Remove when apple clang supports this C++20 feature
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc99-extensions"

const MoveParams moveParams[] = {
  {
    .forwardForce = 80.0f,
    .reverseForce = 50.0f,
    .turnTorque = 200.0f,
    .speed = 15.0f
  }, {
    .forwardForce = 100.0f,
    .reverseForce = 60.0f,
    .turnTorque = 200.0f,
    .speed = 18.0f
  }
};

const MoveUpgrade moveUpgrades[] = {
  {{.cost = 3, .level = 1}},
  {{.cost = -1, .level = -1}}
};

const BlasterParams blasterParams[] = {
  {
    .rof = 1.5f,
    .speed = 60.0f,
    .damage = 5
  }, {
    .rof = 1.5f,
    .speed = 60.0f,
    .damage = 10
  }
};

const BlasterUpgrade blasterUpgrades[] = {
  {{.cost = 4, .level = 1}},
  {{.cost = -1, .level = -1}}
};

const MissileParams missileParams[] = {
  {
    .rof = 0.25f,
    .speed = 25.0f,
    .forwardForce = 25.0f,
    .turnTorque = 0.0f,
    .damage = 20,
    .level = SeekLevel::no_aim
  }, {
    .rof = 0.5f,
    .speed = 25.0f,
    .forwardForce = 25.0f,
    .turnTorque = 3.0f,
    .damage = 20,
    .level = SeekLevel::aim_vel_ahead
  }
};

const MissileUpgrade missileUpgrades[] = {
  {{.cost = 25, .level = 1}},
  {{.cost = -1, .level = -1}}
};

const HullParams hullParams[] = {
  {.durability = 100},
  {.durability = 125},
  {.durability = 150},
  {.durability = 200}
};

const HullUpgrade hullUpgrades[] = {
  {{.cost = 4, .level = 1}},
  {{.cost = 6, .level = 2}},
  {{.cost = 10, .level = 3}},
  {{.cost = -1, .level = -1}}
};

#pragma clang diagnostic pop

bool canUpgrade(Coins &coins, const Upgrade &upgrade) {
  if (upgrade.level == -1) return false;
  if (upgrade.cost > coins.c) return false;
  coins.c -= upgrade.cost;
  return true;
}

}

int motionUpgradeLevels() {
  return std::size(moveParams);
}

void setMotion(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < motionUpgradeLevels());
  reg.assign<MoveParams>(e, moveParams[level]);
  reg.assign<VelocityLimit>(e, moveParams[level].speed);
  reg.assign<MoveCommand>(e);
}

void setUpgradableMotion(entt::registry &reg, const entt::entity e, const int level) {
  setMotion(reg, e, level);
  reg.assign<MoveUpgrade>(e, moveUpgrades[level]);
}

bool upgradeMotion(entt::registry &reg, const entt::entity e) {
  const auto &upgrade = reg.get<MoveUpgrade>(e);
  if (!canUpgrade(reg.get<Coins>(e), upgrade)) return false;
  reg.replace<MoveParams>(e, moveParams[upgrade.level]);
  reg.replace<MoveUpgrade>(e, moveUpgrades[upgrade.level]);
  return true;
}

int blasterUpgradeLevels() {
  return std::size(blasterParams);
}

void setBlaster(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < blasterUpgradeLevels());
  reg.assign<BlasterParams>(e, blasterParams[level]);
  reg.assign<BlasterTimer>(e, std::uint32_t{});
  reg.assign<BlasterCommand>(e);
}

void setUpgradableBlaster(entt::registry &reg, const entt::entity e, const int level) {
  setBlaster(reg, e, level);
  reg.assign<BlasterUpgrade>(e, blasterUpgrades[level]);
}

bool upgradeBlaster(entt::registry &reg, const entt::entity e) {
  const auto &upgrade = reg.get<BlasterUpgrade>(e);
  if (!canUpgrade(reg.get<Coins>(e), upgrade)) return false;
  reg.replace<BlasterParams>(e, blasterParams[upgrade.level]);
  reg.replace<BlasterUpgrade>(e, blasterUpgrades[upgrade.level]);
  return true;
}

int missileUpgradeLevels() {
  return std::size(missileParams);
}

void setMissile(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < missileUpgradeLevels());
  reg.assign<MissileParams>(e, missileParams[level]);
  reg.assign<MissileCommand>(e);
  reg.assign<MissileTimer>(e, std::uint32_t{});
  reg.assign<MissileAmmo>(e, 0);
}

void setUpgradableMissile(entt::registry &reg, const entt::entity e, const int level) {
  setMissile(reg, e, level);
  reg.assign<MissileUpgrade>(e, missileUpgrades[level]);
}

bool upgradeMissile(entt::registry &reg, const entt::entity e) {
  const auto &upgrade = reg.get<MissileUpgrade>(e);
  if (!canUpgrade(reg.get<Coins>(e), upgrade)) return false;
  reg.replace<MissileParams>(e, missileParams[upgrade.level]);
  reg.replace<MissileUpgrade>(e, missileUpgrades[upgrade.level]);
  return true;
}

int hullUpgradeLevels() {
  return std::size(hullParams);
}

void setHull(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < hullUpgradeLevels());
  reg.assign<Hull>(e, hullParams[level].durability);
  reg.assign<HullParams>(e, hullParams[level]);
  reg.assign<BarRect>(e);
}

void setUpgradableHull(entt::registry &reg, const entt::entity e, const int level) {
  setHull(reg, e, level);
  reg.assign<HullUpgrade>(e, hullUpgrades[level]);
}

bool upgradeHull(entt::registry &reg, const entt::entity e) {
  const auto &upgrade = reg.get<HullUpgrade>(e);
  if (!canUpgrade(reg.get<Coins>(e), upgrade)) return false;
  reg.replace<HullParams>(e, hullParams[upgrade.level]);
  reg.replace<HullUpgrade>(e, hullUpgrades[upgrade.level]);
  return true;
}
