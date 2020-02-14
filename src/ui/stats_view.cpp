//
//  stats_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "stats_view.hpp"

#include <SDL_FontCache.h>
#include "../comps/ammo.hpp"
#include "../comps/params.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>
#include "../utils/resource_path.hpp"

StatsView::StatsView(entt::registry &reg)
  : reg{reg} {}

void StatsView::init(SDL_Renderer *ren) {
  font.reset(FC_CreateFont());
  FC_LoadFont(font.get(), ren, res("FreeSans.ttf"), 16, {255, 255, 255, 255}, TTF_STYLE_NORMAL);
}

bool StatsView::event(const SDL_Event &, const SDL_Rect) {
  return false;
}

void StatsView::render(SDL_Renderer *ren, const SDL_Rect) {
  auto view = reg.view<Hull, HullParams, CameraFocus>();
  view.less([&](auto hull, auto params) {
    FC_Draw(font.get(), ren, 0.0f, 0.0f, "Hull: %d/%d", hull.h, params.durability);
  });
}
