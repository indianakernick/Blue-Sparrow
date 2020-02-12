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

/// Read the MoveInput and MoveParams to apply forces to the Physics body
void applyMoveCommands(entt::registry &);

/// Read the BlasterInput and BlasterParams to fire the blaster
void applyBlasterCommands(entt::registry &);

/// Read the MissileInput and MissileParams to fire a missile
void applyMissileCommands(entt::registry &);

#endif
