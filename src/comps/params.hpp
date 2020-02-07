//
//  params.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef params_hpp
#define params_hpp

/// Forces and torques used to move the physics body.
/// Can be changed when the player upgrades their ship.
struct MoveParams {
  float thrustForce;
  float turnTorque;
};

#endif
