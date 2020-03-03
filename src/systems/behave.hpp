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

// See behaviour desciptions in comps/behaviour.hpp
// See mouse and aim assist descriptions in comps/input.hpp

// TODO: Try to simplify behaviours
// Use less components in each of the systems
// Try to split the behaviours into multiple behaviours if possible
// Ideally, moving/aiming/shooting would be in separate systems

// Navigate should have a flag for looking where it's going and aiming
// AI should look where it's going if there are no targets nearby
// AI should add an aiming behaviour if there are targets nearby

// We seem to check if the target is null in most behaviours
// Can we avoid this?
// Maybe the AI should change behaviour if there are not targets

void behaveOrbit(entt::registry &);
void behaveAim(entt::registry &);
void behaveSeek(entt::registry &);
void behaveStationary(entt::registry &);
void behaveMouse(entt::registry &);
void behaveNavigate(entt::registry &);
void behaveIdle(entt::registry &);
void behavePacifist(entt::registry &);

#endif
