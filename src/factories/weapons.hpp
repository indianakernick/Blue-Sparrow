//
//  weapons.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_weapons_hpp
#define factories_weapons_hpp

#include "../comps/teams.hpp"
#include <entt/entity/fwd.hpp>

entt::entity makeBolt(entt::registry &, Team);
entt::entity makeMissile(entt::registry &, Team);

#endif
