//
//  params.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_params_hpp
#define comps_params_hpp

#include "ai.hpp"

struct Upgrade {
  int cost;
  int level;
};

struct MoveUpgrade : Upgrade {};
struct BlasterUpgrade : Upgrade {};
struct MissileUpgrade : Upgrade {};
struct HullUpgrade : Upgrade {};

// TODO: Perhaps rename MoveParams to MotionParams

/// Forces and torques used to move the physics body.
/// Can be changed when the player upgrades their ship.
struct MoveParams {
  float forwardForce;
  float reverseForce;
  float turnTorque;
  float speed;
};

/// Properties of the primary weapon.
/// Can be changed when the player upgrades their ship.
struct BlasterParams {
  float rof;
  float speed;
  int damage;
  float spread;
};

/// Properties of the secondary weapon.
/// Can be changed when the player upgrades their ship.
struct MissileParams {
  float rof;
  float speed;
  float forwardForce;
  float turnTorque;
  int damage;
  SeekLevel level;
};

/// The maximum hull durability.
/// Can be changed when the player upgrades their ship.
struct HullParams {
  int durability;
};

#endif
