//
//  input.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef input_hpp
#define input_hpp

/// If present, the keyboard will control MoveInput
struct KeyInput {};

/// Influences torque and forces applied to the physics body.
/// Controlled by keyboard or AI.
struct MoveInput {
  bool forward = false;
  bool left = false;
  bool right = false;
};

/// Causes the blaster to be fired.
/// Controlled by keyboard or AI.
struct BlasterInput {
  bool fire = false;
};

#endif
