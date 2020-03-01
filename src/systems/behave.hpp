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

void behaveOrbit(entt::registry &);
void behaveSeek(entt::registry &);
void behaveSniper(entt::registry &);
void behaveStationary(entt::registry &);
void behaveMouse(entt::registry &);
void behaveNavigate(entt::registry &);
void behaveIdle(entt::registry &);
void behavePacifist(entt::registry &);

#endif
