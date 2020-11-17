//
//  constants.hpp
//  Blue Sparrow
//
//  Created by Indiana Kernick on 17/11/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef config_constants_hpp
#define config_constants_hpp

#include <cstdint>

/// The amount of time that the navigate behaviour will try to reach the next
/// point before giving up and running the path finder again.
constexpr std::uint32_t navigate_retry_time = 1500;
/// The distance from the first path point to reach before targetting the next
/// point.
constexpr float navigate_first_point_dist = 2.0f;
/// The distance from the second path point to reach before skipping the first
/// point and targetting the second.
constexpr float navigate_second_point_dist = 6.0f;

/// The amount of time a bolt will live for before expiring
constexpr std::uint32_t bolt_lifetime = 5000;
/// The amount of time a missle will live for before expiring
constexpr std::uint32_t missle_lifetime = 15000;

#endif
