//
//  arena.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 21/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_arena_hpp
#define comps_arena_hpp

#include <vector>

enum class BeaconState {
  ally,
  enemy,
  neutral
};

/// Beacons must be damaged to be captured
struct Beacon {
  int max;
  int ally;
  int enemy;
  int neutral;
  BeaconState state;
  int order;
};

struct MapData {
  std::vector<bool> data;
  int width, height;
  int scale;
};

#endif
