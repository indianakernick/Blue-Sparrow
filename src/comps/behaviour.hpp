//
//  behaviour.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_behaviour_hpp
#define comps_behaviour_hpp

#include <vector>
#include "levels.hpp"
#include <box2d/b2_math.h>
#include <entt/entity/fwd.hpp>
#include <entt/entity/entity.hpp>

/// The target that this entity is observing/shooting/chasing
struct Target {
  entt::entity e = entt::null;
};

/// Maintain a fixed distance from the target
struct OrbitBehaviour {
  float dist;
  float speed;
};

/// Aim at target
struct AimBehaviour {
  AimLevel level;
  /// The minimum angle desired to fire rotate the ship
  float rotateThreshold = 0.5;
  /// The maximum angle difference to fire the blasters
  float fireThreshold = b2_pi / 8.0f;
};

/// Collide with the target
struct SeekBehaviour {
  float speed;
  SeekLevel level;
  /// The maximum angle difference to fire the forward thruster
  float fireThreshold = b2_pi / 8.0f;
};

/// Remain stationary at a location
struct StationaryBehaviour {
  b2Vec2 pos = {0.0f, 0.0f};
  /// The minimum linear acceleration desired to fire the thrusters
  float moveThreshold = 0.5f;
};

/// Path-find to a location in the arena
struct NavigateBehaviour {
  b2Vec2 target;
  std::vector<b2Vec2> path;
  std::uint32_t timeout = 0;
  bool look = true;
  
  entt::entity debug0 = entt::null;
  entt::entity debug1 = entt::null;
};

/// Don't move
struct IdleBehaviour {};

/// Don't shoot
struct PacifistBehaviour {};

#endif
