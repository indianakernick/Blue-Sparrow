//
//  map_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/3/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "map_view.hpp"

#include "../comps/arena.hpp"
#include "../systems/camera.hpp"
#include "../systems/render.hpp"
#include <entt/entity/registry.hpp>

MapView::MapView(entt::registry &reg)
  : reg{reg} {}

void MapView::init(SDL_Renderer *, FontCache &) {
  constexpr int scale = 4;
  const auto &map = reg.ctx<MapWalls>();
  setWidth({map.width * scale, 0, 0});
  setHeight({map.height * scale, 0, 0});
  camera.x = (map.width * map.scale) / -2.0f;
  camera.y = (map.height * map.scale) / -2.0f;
  camera.zoom = static_cast<float>(scale) / map.scale;
  camera.width = camera.height = 0.0f;
  camera.arenaWidth = camera.arenaHeight = 0.0f;
  camera.minZoom = 0.0f;
}

void MapView::render(SDL_Renderer *, FontCache &) {
  // TODO: Just get the SpriteRect of player and beacons
  writeSpriteRect(reg, camera);
  renderMap(reg);
}
