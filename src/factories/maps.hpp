//
//  maps.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 22/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_maps_hpp
#define factories_maps_hpp

#include <entt/entity/fwd.hpp>
#include "../utils/load_image.hpp"

// TODO: Perhaps separate the map from the game mode
// A function for place walls and a function for placing beacons

struct MapInfo {
  float width, height;
  Image image;
};

MapInfo makeMap0(entt::registry &);

#endif
