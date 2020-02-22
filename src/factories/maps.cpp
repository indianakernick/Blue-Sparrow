//
//  maps.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 22/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "maps.hpp"

#include "ships.hpp"
#include "arena.hpp"
#include "../systems/physics.hpp"
#include "../utils/load_image.hpp"
#include "../utils/resource_path.hpp"

MapInfo makeMap0(entt::registry &reg) {
  constexpr float scale = 5.0f;

  const MapInfo info = {makePlayer(reg), 300.0f, 200.0f};
  Image image = loadImage(res("battle_arena_map_0.png"));
  assert(image.width * scale == info.width);
  assert(image.height * scale == info.height);

  makeArena(reg, info.width, info.height);
  
  const unsigned char *pixel = image.data.get();
  for (int y = 0; y != image.height; ++y) {
    for (int x = 0; x != image.width; ++x) {
      if (pixel[0] == 128 && pixel[1] == 128 && pixel[2] == 128 && pixel[3] == 255) {
        const float wallW = scale;
        const float wallH = scale;
        const float wallX = (x + 0.5f) * scale - info.width / 2.0f;
        const float wallY = (y + 0.5f) * scale - info.height / 2.0f;
        setTransform(reg, makeWall(reg, wallW, wallH), {wallX, wallY}, 0.0f);
      }
      pixel += 4;
    }
  }

  setTransform(reg, makeBeacon(reg, BeaconState::ally), {-27.0f * scale, 0.0f}, 0.0f);
  setTransform(reg, makeBeacon(reg, BeaconState::enemy), {27.0f * scale, 0.0f}, 0.0f);
  setTransform(reg, makeBeacon(reg, BeaconState::neutral), {0.0f, -13.0f * scale}, 0.0f);
  setTransform(reg, makeBeacon(reg, BeaconState::neutral), {0.0f, 13.0f * scale}, 0.0f);

  setTransform(reg, info.player, {-29.0f * scale, 0.0f}, 0.0f);
  setTransform(reg, makeScout(reg, Team::enemy), {29.0f * scale, 0.0f}, b2_pi);
  
  entt::entity rock = makeAsteroid(reg);
  setTransform(reg, rock, {0.0f, 0.0f}, 1.0f);
  setMotion(reg, rock, {1.0f, 1.3f}, 0.1f);

  return info;
}
