//
//  behave.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_behave_hpp
#define systems_behave_hpp

#include <entt/entity/fwd.hpp>

/// Maintain a fixed distance from the target and shoot
void behaveOrbit(entt::registry &);
/// Collide with the target
void behaveSeek(entt::registry &);
/// Remain stationary and aim carefully
void behaveSniper(entt::registry &);
/// Rotate to mouse position
void behaveMouse(entt::registry &);

// Maybe a turret behaviour that stays still and rotates to shoot
// It could try to move back to it
// Sniper behaviour with fast blaster and slow rotation

#endif
