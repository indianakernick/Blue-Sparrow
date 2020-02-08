//
//  behaviour.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_behaviour_hpp
#define comps_behaviour_hpp

#include <entt/entity/fwd.hpp>

struct Target {
  entt::entity e;
};

struct OrbitBehaviour {
  float dist;
};

struct SeekBehaviour {};

#endif
