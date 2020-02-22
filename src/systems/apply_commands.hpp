//
//  apply_commands.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_apply_commands_hpp
#define systems_apply_commands_hpp

#include <entt/entity/fwd.hpp>

/// Read the MotionCommand and MotionParams to apply forces to the Physics body
void applyMotionCommands(entt::registry &);

/// Read the BlasterCommand and BlasterParams to fire the blaster
void applyBlasterCommands(entt::registry &);

/// Read the MissileCommand and MissileParams to fire a missile
void applyMissileCommands(entt::registry &);

#endif
