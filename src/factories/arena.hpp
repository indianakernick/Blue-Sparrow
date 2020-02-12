//
//  arena.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 9/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_arena_hpp
#define factories_arena_hpp

#include <entt/entity/fwd.hpp>

entt::entity makeArena(entt::registry &, float, float);
entt::entity makeAsteroid(entt::registry &);
entt::entity makeCoin(entt::registry &);
entt::entity makeAmmo(entt::registry &);

#endif
