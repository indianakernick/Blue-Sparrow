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
/// Rotate to mouse position and adjust aim if AimAssist is present
void behaveMouse(entt::registry &);
/// Move to some location on the map
void behaveNavigate(entt::registry &);

#endif
