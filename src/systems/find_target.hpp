//
//  find_target.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_find_target_hpp
#define systems_find_target_hpp

#include <entt/entity/fwd.hpp>

/// Search for the nearest opposing ship
entt::entity findNearestEnemyShip(entt::registry &, entt::entity);

#endif
