//
//  ai.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 11/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_ai_hpp
#define comps_ai_hpp

/// "Smartness" level for OrbitBehaviour
enum class OrbitLevel {
  /// Aim at the position of the target
  aim_pos,
  /// Aim ahead of the target
  aim_ahead
};

/// "Smartness" level for SeekBehaviour
enum class SeekLevel {
  /// Travel in a straight line
  no_aim,
  /// Aim at the position of the target
  aim_pos,
  /// Aim at the position of the target while accounting for velocity
  aim_vel_pos,
  /// Aim at the future position of the target while accounting for velocity
  aim_vel_ahead
};

#endif
