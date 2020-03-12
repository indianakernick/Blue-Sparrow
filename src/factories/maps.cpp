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
#include "../comps/ai.hpp"
#include "../comps/behaviour.hpp"
#include "../systems/physics.hpp"
#include "../utils/load_image.hpp"
#include <entt/entity/registry.hpp>
#include "../utils/resource_path.hpp"

namespace {

void setMapSize(MapWalls &map, const MapInfo &info, const float scale) {
  map.width = info.image.width;
  map.height = info.image.height;
  map.data.resize(map.width * map.height);
  map.scale = scale;
}

void initializeWalls(entt::registry &reg, MapWalls &map, const MapInfo &info) {
  const unsigned char *pixel = info.image.data.get();
  auto tile = map.data.begin();
  for (int y = 0; y != info.image.height; ++y) {
    for (int x = 0; x != info.image.width; ++x) {
      if (pixel[3] != 0) {
        const float wallW = map.scale;
        const float wallH = map.scale;
        const float wallX = (x + 0.5f) * map.scale - info.width / 2.0f;
        const float wallY = (y + 0.5f) * map.scale - info.height / 2.0f;
        setTransform(reg, makeWall(reg, wallW, wallH), {wallX, wallY}, 0.0f);
        *tile = true;
      }
      pixel += 4;
      ++tile;
    }
  }
}

template <std::size_t Size>
void initializeBeacons(
  entt::registry &reg,
  MapWalls &map,
  const MapInfo &info,
  const b2Vec2 (&beacons)[Size]
) {
  for (std::size_t b = 0; b != Size; ++b) {
    BeaconState state;
    if (b == 0) {
      state = BeaconState::ally;
    } else if (b == Size - 1) {
      state = BeaconState::enemy;
    } else {
      state = BeaconState::neutral;
    }
    const entt::entity beacon = makeBeacon(reg, state, static_cast<int>(b));
    setTransform(reg, beacon, map.scale * beacons[b], 0.0f);
    
    b2Vec2 pos = beacons[b];
    pos += b2Vec2{info.image.width / 2.0f, info.image.height / 2.0f};
    const std::size_t index = pos.y * info.image.width + pos.x;
    map.data[index] = true;
    map.data[index - 1] = true;
    map.data[index - info.image.width] = true;
    map.data[index - info.image.width - 1] = true;
  }
}

void initializeAllies(entt::registry &reg, const b2Vec2 pos, const int size) {
  setTransform(reg, makePlayer(reg, true), pos, 0.0f);
  for (int s = 1; s < size; ++s) {
    setTransform(reg, makeScout(reg, Team::ally), pos, 0.0f);
  }
}

void initializeEnemies(entt::registry &reg, const b2Vec2 pos, const int size) {
  for (int s = 0; s < size; ++s) {
    setTransform(reg, makeScout(reg, Team::enemy), pos, 0.0f);
  }
}

}

MapInfo makeMap0(entt::registry &reg) {
  const int scale = 5;

  MapInfo info = {
    300.0f, 200.0f,
    loadImage(res("battle_arena_map_0.png"))
  };
  assert(info.image.width * scale == info.width);
  assert(info.image.height * scale == info.height);
  
  const b2Vec2 beacons[] = {
    {-27.0f, 0.0f}, {0.0f, -13.0f}, {0.0f, 13.0f}, {27.0f, 0.0f}
  };
  const b2Vec2 allySpawn = {scale * -29.0f, 0.0f};
  const b2Vec2 enemySpawn = {scale * 29.0f, 0.0f};
  const int teamSize = 5;
  
  auto &map = reg.set<MapWalls>();
  setMapSize(map, info, scale);
  initializeWalls(reg, map, info);
  initializeBeacons(reg, map, info, beacons);
  initializeAllies(reg, allySpawn, teamSize);
  initializeEnemies(reg, enemySpawn, teamSize);
  makeArena(reg, info.width, info.height);
  
  /*entt::entity scout = makeScout(reg, Team::ally);
  setTransform(reg, scout, {-29.0f * scale, 0.0f}, 0.0f);*/
  
  /*reg.remove<BeaconCaptureAI>(scout);
  reg.assign<NavigateBehaviour>(
    scout,
    //29.0f * scale, 19.0f * scale,
    29.0f * scale, 0.0f,
    std::vector<b2Vec2>(),
    makeDebugPoint(reg), makeDebugPoint(reg)
  );*/
  
  entt::entity sniper = makeSniper(reg, Team::enemy);
  setSniperPosition(reg, sniper, 6.0f * scale, 0.0f);
  setTransform(reg, sniper, {6.0f * scale, 0.0f}, b2_pi);
  
  //entt::entity rock = makeAsteroid(reg);
  //setTransform(reg, rock, {0.0f, 0.0f}, 1.0f);
  //setMotion(reg, rock, {1.0f, 1.3f}, 0.1f);

  return info;
}
