//
//  input.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_input_hpp
#define comps_input_hpp

/// If present, the keyboard will control MoveInput
struct KeyInput {};

/// Influences forces and torques applied to the physics body.
struct MoveInput {
  bool forward = false;
  bool reverse = false;
  bool left = false;
  bool right = false;
};

/// Causes the blaster to be fired.
struct BlasterInput {
  bool fire = false;
};

/// Causes a missile to be fired.
struct MissileInput {
  bool fire = false;
};

#endif
