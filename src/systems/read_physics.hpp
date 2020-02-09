//
//  read_physics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_read_physics_hpp
#define systems_read_physics_hpp

#include <entt/entity/fwd.hpp>

/// Read the position and angle of Physics bodies and update the SpriteRect
void readPhysicsTransform(entt::registry &);

#endif
