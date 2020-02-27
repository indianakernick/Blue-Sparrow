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

/// The target that this entity is observing/shooting/chasing
struct Target {
  entt::entity e = entt::null;
};

/// Search for the nearest visible enemy ship
struct TargetEnemyShip {
  /// If true, always try to find the closest target even if a target has
  /// already been aquired
  bool eager = false;
};

/// Search for the nearest neutral or enemy beacon
struct TargetBeacon {};

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
