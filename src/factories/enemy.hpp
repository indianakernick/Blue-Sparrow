//
//  enemy.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_enemy_hpp
#define factories_enemy_hpp

#include "../comps/teams.hpp"
#include <entt/entity/fwd.hpp>

entt::entity makeEnemy(entt::registry &, Team);

#endif
