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
#include <entt/entity/entity.hpp>

struct Target {
  bool eager = false;
  entt::entity e = entt::null;
};

struct OrbitBehaviour {
  float dist;
  float speed;
};

struct SeekBehaviour {
  float speed;
};

#endif
