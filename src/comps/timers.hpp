//
//  timers.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_timers_hpp
#define comps_timers_hpp

#include <cstdint>

/// Timer used for limiting the ROF of a blaster
struct BlasterTimer {
  std::uint32_t done;
};

/// Timer used for limiting the ROF of a missile
struct MissileTimer {
  std::uint32_t done;
};

/// Timer used for destroying objects after some amount of time
struct ExpireTimer {
  std::uint32_t done;
};

/// Context variable for the current time
struct Now {
  std::uint32_t time;
};

// This is basically SDL_TICKS_PASSED
constexpr bool timePassed(const std::uint32_t now, const std::uint32_t timeout) {
  return static_cast<std::int32_t>(timeout - now) <= 0;
}

#endif
