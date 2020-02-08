//
//  params.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_params_hpp
#define comps_params_hpp

/// Forces and torques used to move the physics body.
/// Can be changed when the player upgrades their ship.
struct MoveParams {
  float forwardForce;
  float reverseForce;
  float turnTorque;
};

/// Properties of the players primary weapon.
/// Can be changed when the player upgrades their ship.
struct BlasterParams {
  float rof;
  float speed;
};

#endif
