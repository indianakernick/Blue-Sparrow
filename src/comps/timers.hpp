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

/// Timer used for limited the ROF of a blaster
struct BlasterTimer {
  std::uint32_t done;
};

/// Timer used for destroying objects after some amount of time
struct ExpireTimer {
  std::uint32_t done;
};

#endif
