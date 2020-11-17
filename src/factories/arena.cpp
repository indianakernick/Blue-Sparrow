//
//  arena.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 9/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "arena.hpp"

#include "physics.hpp"
#include "../comps/arena.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeArena(entt::registry &reg, const float width, const float height) {
  entt::entity e = reg.create();
  setArenaPhysics(reg, e, width, height);
  reg.emplace<SpriteRect>(e);
  reg.emplace<BackgroundSprite>(e);
  return e;
}

entt::entity makeAsteroid(entt::registry &reg) {
  entt::entity e = reg.create();
  setAsteroidPhysics(reg, e);
  reg.emplace<SpriteRect>(e);
  reg.emplace<Sprite>(e, Sprite{129, 76, 38});
  return e;
}

entt::entity makeBeacon(entt::registry &reg, const BeaconState state, const int order) {
  entt::entity e = reg.create();
  setBeaconPhysics(reg, e);
  
  Beacon beacon;
  beacon.max = 1000;
  beacon.ally = 0;
  beacon.enemy = 0;
  beacon.neutral = 0;
  beacon.state = state;
  beacon.order = order;
  
  switch (state) {
    case BeaconState::ally:
      beacon.ally = beacon.max;
      break;
    case BeaconState::enemy:
      beacon.enemy = beacon.max;
      break;
    case BeaconState::neutral:
      beacon.neutral = beacon.max;
      break;
  }
  
  reg.emplace<Beacon>(e, beacon);
  reg.emplace<BarRect>(e);
  reg.emplace<SpriteRect>(e);
  reg.emplace<Sprite>(e);
  return e;
}

entt::entity makeWall(entt::registry &reg, const float width, const float height) {
  entt::entity e = reg.create();
  setWallPhysics(reg, e, width, height);
  reg.emplace<SpriteRect>(e);
  reg.emplace<Sprite>(e, Sprite{191, 191, 191});
  return e;
}

entt::entity makeDebugPoint(entt::registry &reg) {
  entt::entity e = reg.create();
  setDebugPointPhysics(reg, e);
  reg.emplace<SpriteRect>(e);
  reg.emplace<Sprite>(e, Sprite{255, 0, 255});
  return e;
}
