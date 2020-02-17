//
//  stats_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "stats_view.hpp"

#include "font_cache.hpp"
#include "../comps/ammo.hpp"
#include "../comps/params.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>
#include "../utils/resource_path.hpp"

StatsView::StatsView(entt::registry &reg)
  : reg{reg} {
  setFixedWidth(200);
  setGrowHeight(100);
}

void StatsView::init(SDL_Renderer *, FontCache &cache) {
  font = cache.load(res("FreeSans.ttf"), 16);
}

bool StatsView::event(const SDL_Event &) {
  return false;
}

void StatsView::render(SDL_Renderer *, FontCache &cache) {
  auto view = reg.view<Hull, HullParams, Coins, MissileAmmo, CameraFocus>();
  view.less([&](auto hull, auto params, auto coins, auto ammo) {
    cache.draw(font, 0.0f, 0.0f, "Hull: %d/%d", hull.h, params.durability);
    cache.draw(font, 0.0f, 18.0f, "Coins: %d", coins.c);
    cache.draw(font, 0.0f, 36.0f, "Missiles: %d", ammo.n);
  });
}
