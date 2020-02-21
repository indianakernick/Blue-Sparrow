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
#include "../comps/drops.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

entt::entity makeArena(entt::registry &reg, const float width, const float height) {
  entt::entity e = reg.create();
  setArenaPhysics(reg, e, width, height);
  reg.assign<SpriteRect>(e);
  reg.assign<BackgroundSprite>(e);
  return e;
}

entt::entity makeAsteroid(entt::registry &reg) {
  entt::entity e = reg.create();
  setAsteroidPhysics(reg, e);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{129, 76, 38});
  return e;
}

entt::entity makeCoin(entt::registry &reg) {
  entt::entity e = reg.create();
  setDropPhysics(reg, e);
  reg.assign<Coin>(e);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{255, 255, 0});
  return e;
}

entt::entity makeAmmo(entt::registry &reg) {
  entt::entity e = reg.create();
  setDropPhysics(reg, e);
  reg.assign<Ammo>(e);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{191, 191, 191});
  return e;
}

entt::entity makeScrap(entt::registry &reg) {
  entt::entity e = reg.create();
  setDropPhysics(reg, e);
  reg.assign<Scrap>(e);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{63, 63, 63});
  return e;
}

entt::entity makeBeacon(entt::registry &reg) {
  entt::entity e = reg.create();
  setBeaconPhysics(reg, e);
  Beacon beacon;
  beacon.max = 1000;
  beacon.ally = 0;
  beacon.enemy = 0;
  beacon.neutral = beacon.max;
  beacon.state = BeaconState::neutral;
  reg.assign<Beacon>(e, beacon);
  reg.assign<BarRect>(e);
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{255, 255, 0});
  return e;
}
