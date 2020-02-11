//
//  physics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_physics_hpp
#define factories_physics_hpp

#include "../comps/teams.hpp"
#include <entt/entity/fwd.hpp>

void setArenaPhysics(entt::registry &, entt::entity, float, float);
void setSmallShipPhysics(entt::registry &, entt::entity, Team);
void setSmallBoltPhysics(entt::registry &, entt::entity, Team);
void setSmallMissilePhysics(entt::registry &, entt::entity, Team);
void setAsteroidPhysics(entt::registry &, entt::entity);

#endif
