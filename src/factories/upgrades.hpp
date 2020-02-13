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

int motionUpgradeLevels();
void setMotion(entt::registry &, entt::entity, int);
void setUpgradableMotion(entt::registry &, entt::entity, int);
bool upgradeMotion(entt::registry &, entt::entity);

int blasterUpgradeLevels();
void setBlaster(entt::registry &, entt::entity, int);
void setUpgradableBlaster(entt::registry &, entt::entity, int);
bool upgradeBlaster(entt::registry &, entt::entity);

int missileUpgradeLevels();
void setMissile(entt::registry &, entt::entity, int);
void setUpgradableMissile(entt::registry &, entt::entity, int);
bool upgradeMissile(entt::registry &, entt::entity);

int hullUpgradeLevels();
void setHull(entt::registry &, entt::entity, int);
void setUpgradableHull(entt::registry &, entt::entity, int);
bool upgradeHull(entt::registry &, entt::entity);

#endif
