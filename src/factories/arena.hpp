//
//  arena.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 9/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_arena_hpp
#define factories_arena_hpp

#include "../comps/arena.hpp"
#include <entt/entity/fwd.hpp>

entt::entity makeArena(entt::registry &, float, float);
entt::entity makeAsteroid(entt::registry &);
entt::entity makeBeacon(entt::registry &, BeaconState, int);
entt::entity makeWall(entt::registry &, float, float);
entt::entity makeDebugPoint(entt::registry &);

#endif
