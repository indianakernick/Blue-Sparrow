//
//  collisions.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_collisions_hpp
#define systems_collisions_hpp

#include <entt/entity/fwd.hpp>

/// Handle the collisions that occured during the last time step (BeginContact)
void handleCollisions(entt::registry &);

/// Handle the collisions that occured during the last time step (PostSolve)
void handlePostCollisions(entt::registry &);

#endif
