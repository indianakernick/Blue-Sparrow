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

int hullUpgradeLevels();
void setHull(entt::registry &, entt::entity, int);
void setUpgradableHull(entt::registry &, entt::entity, int);
bool upgradeHull(entt::registry &, entt::entity, int);

#endif
