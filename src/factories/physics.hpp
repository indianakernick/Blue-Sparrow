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
#include "../comps/physics.hpp"

class b2World;

Physics makeSmallShip(b2World &, Team);
Physics makeSmallBullet(b2World &, Team);
Physics makeSmallMissile(b2World &, Team);

#endif
