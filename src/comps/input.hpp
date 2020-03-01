//
//  input.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_input_hpp
#define comps_input_hpp

/// If present, the keyboard will control MotionCommand
struct KeyInput {};

/// If present, the mouse will control MotionCommand.
/// The ship will rotate to point to the mouse position and aim is adjusted if
/// AimAssist is present.
struct MouseInput {
  int x = 0;
  int y = 0;
};

/// If present, the ship will be rotated towards the enemy ship directly under
/// the mouse.
struct AimAssist {};

/// Influences forces and torques applied to the physics body.
struct MotionCommand {
  bool forward = false;
  bool reverse = false;
  bool ccw = false;
  bool cw = false;
  bool left = false;
  bool right = false;
};

/// Causes the blaster to be fired.
struct BlasterCommand {
  bool fire = false;
};

/// Causes a missile to be fired.
struct MissileCommand {
  bool fire = false;
};

#endif
