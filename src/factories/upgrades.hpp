//
//  upgrades.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 13/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_upgrades_hpp
#define factories_upgrades_hpp

#include <entt/entity/fwd.hpp>

struct UpgradeInfo {
  int level;
  int total;
  int cost;
  bool able;
};

bool upgradeMotion(entt::registry &);
bool motionUpgradeInfo(entt::registry &, UpgradeInfo &);
void setMotion(entt::registry &, entt::entity, int);
void setUpgradableMotion(entt::registry &, entt::entity, int);

bool upgradeBlaster(entt::registry &);
bool blasterUpgradeInfo(entt::registry &, UpgradeInfo &);
void setBlaster(entt::registry &, entt::entity, int);
void setUpgradableBlaster(entt::registry &, entt::entity, int);

bool upgradeMissile(entt::registry &);
bool missileUpgradeInfo(entt::registry &, UpgradeInfo &);
void setMissile(entt::registry &, entt::entity, int);
void setUpgradableMissile(entt::registry &, entt::entity, int);

bool upgradeHull(entt::registry &);
bool hullUpgradeInfo(entt::registry &, UpgradeInfo &);
void setHull(entt::registry &, entt::entity, int);
void setUpgradableHull(entt::registry &, entt::entity, int);

#endif
