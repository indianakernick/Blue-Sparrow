//
//  behaviour.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_behaviour_hpp
#define comps_behaviour_hpp

#include "ai.hpp"
#include <vector>
#include <box2d/b2_math.h>
#include <entt/entity/fwd.hpp>
#include <entt/entity/entity.hpp>

struct Target {
  /// If true, always try to find the closest target even if a target has
  /// already been aquired
  bool eager = false;
  entt::entity e = entt::null;
};

/// Maintain a fixed distance from the target
struct OrbitBehaviour {
  float dist;
  float speed;
  OrbitLevel level;
};

/// Collide with the target
struct SeekBehaviour {
  float speed;
  SeekLevel level;
};

/// Remain at a given location and aim carefully
struct SniperBehaviour {
  float x, y;
};

struct NavigateBehaviour {
  float x, y;
  std::vector<b2Vec2> path;
  entt::entity debug0 = entt::null;
  entt::entity debug1 = entt::null;
};

#endif
