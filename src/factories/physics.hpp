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
#include "../comps/physics.hpp"

class b2World;

Physics makeArena(b2World &, float, float);
Physics makeSmallShip(b2World &, Team, entt::entity);
Physics makeSmallBullet(b2World &, Team, entt::entity);
Physics makeSmallMissile(b2World &, Team, entt::entity);

//void setUserData(b2Body *, entt::entity);

#endif
