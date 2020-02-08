//
//  missile.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_missile_hpp
#define factories_missile_hpp

#include "../comps/teams.hpp"
#include <entt/entity/fwd.hpp>

entt::entity makeMissile(entt::registry &, Team);

#endif
