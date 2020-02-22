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

const MotionParams motionParams[] = {
  {
    .forwardForce = 80.0f,
    .reverseForce = 50.0f,
    .lateralForce = 70.0f,
    .turnTorque = 200.0f,
    .speed = 15.0f
  }, {
    .forwardForce = 100.0f,
    .lateralForce = 90.0f,
    .reverseForce = 60.0f,
    .turnTorque = 200.0f,
    .speed = 18.0f
  }
};

const MotionUpgrade motionUpgrades[] = {
  {{.cost = 3, .level = 1}},
  {{.cost = -1, .level = 2}}
};

const BlasterParams blasterParams[] = {
  {
    .rof = 1.5f,
    .speed = 60.0f,
    .damage = 5,
    .spread = 0.05f
  }, {
    .rof = 1.5f,
    .speed = 60.0f,
    .damage = 10,
    .spread = 0.05f
  }
};

const BlasterUpgrade blasterUpgrades[] = {
  {{.cost = 4, .level = 1}},
  {{.cost = -1, .level = 2}}
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
  {{.cost = -1, .level = 2}}
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
  {{.cost = -1, .level = 4}}
};

#pragma clang diagnostic pop

template <typename Upgrade, typename Params, size_t Size>
bool upgrade(entt::registry &reg, const Upgrade (&upgrades)[Size], const Params (&params)[Size]) {
  bool success = false;
  auto view = reg.view<Upgrade, Params, Coins>();
  view.each([&](auto &upgrade, auto &param, auto &coins) {
    if (upgrade.cost == -1) return;
    if (upgrade.cost > coins.c) return;
    coins.c -= upgrade.cost;
    param = params[upgrade.level];
    upgrade = upgrades[upgrade.level];
    success = true;
  });
  return success;
}

template <typename Upgrade, size_t Size>
bool getInfo(entt::registry &reg, UpgradeInfo &info, const Upgrade (&)[Size]) {
  bool success = false;
  auto view = reg.view<Upgrade, Coins>();
  view.each([&](auto upgrade, auto coins) {
    info.total = Size;
    info.level = upgrade.level;
    info.cost = upgrade.cost;
    info.able = upgrade.cost != -1 && upgrade.cost <= coins.c;
    success = true;
  });
  return success;
}

}

bool upgradeMotion(entt::registry &reg) {
  return upgrade(reg, motionUpgrades, motionParams);
}

bool motionUpgradeInfo(entt::registry &reg, UpgradeInfo &info) {
  return getInfo(reg, info, motionUpgrades);
}

void setMotion(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < int{std::size(motionParams)});
  reg.assign<MotionParams>(e, motionParams[level]);
  reg.assign<VelocityLimit>(e, motionParams[level].speed);
  reg.assign<MotionCommand>(e);
}

void setUpgradableMotion(entt::registry &reg, const entt::entity e, const int level) {
  setMotion(reg, e, level);
  reg.assign<MotionUpgrade>(e, motionUpgrades[level]);
}

bool upgradeBlaster(entt::registry &reg) {
  return upgrade(reg, blasterUpgrades, blasterParams);
}

bool blasterUpgradeInfo(entt::registry &reg, UpgradeInfo &info) {
  return getInfo(reg, info, blasterUpgrades);
}

void setBlaster(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < int{std::size(blasterParams)});
  reg.assign<BlasterParams>(e, blasterParams[level]);
  reg.assign<BlasterTimer>(e, std::uint32_t{});
  reg.assign<BlasterCommand>(e);
}

void setUpgradableBlaster(entt::registry &reg, const entt::entity e, const int level) {
  setBlaster(reg, e, level);
  reg.assign<BlasterUpgrade>(e, blasterUpgrades[level]);
}

bool upgradeMissile(entt::registry &reg) {
  return upgrade(reg, missileUpgrades, missileParams);
}

bool missileUpgradeInfo(entt::registry &reg, UpgradeInfo &info) {
  return getInfo(reg, info, missileUpgrades);
}

void setMissile(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < int{std::size(missileParams)});
  reg.assign<MissileParams>(e, missileParams[level]);
  reg.assign<MissileCommand>(e);
  reg.assign<MissileTimer>(e, std::uint32_t{});
  reg.assign<MissileAmmo>(e, 0);
}

void setUpgradableMissile(entt::registry &reg, const entt::entity e, const int level) {
  setMissile(reg, e, level);
  reg.assign<MissileUpgrade>(e, missileUpgrades[level]);
}

bool upgradeHull(entt::registry &reg) {
  return upgrade(reg, hullUpgrades, hullParams);
}

bool hullUpgradeInfo(entt::registry &reg, UpgradeInfo &info) {
  return getInfo(reg, info, hullUpgrades);
}

void setHull(entt::registry &reg, const entt::entity e, const int level) {
  assert(level < int{std::size(hullParams)});
  reg.assign<Hull>(e, hullParams[level].durability);
  reg.assign<HullParams>(e, hullParams[level]);
  reg.assign<BarRect>(e);
}

void setUpgradableHull(entt::registry &reg, const entt::entity e, const int level) {
  setHull(reg, e, level);
  reg.assign<HullUpgrade>(e, hullUpgrades[level]);
}
