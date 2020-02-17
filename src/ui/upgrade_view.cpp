//
//  upgrade_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "upgrade_view.hpp"

#include <SDL_FontCache.h>
#include "../comps/params.hpp"
#include "../utils/sdl_check.hpp"
#include <entt/entity/registry.hpp>
#include "../factories/upgrades.hpp"
#include "../utils/resource_path.hpp"

UpgradeView::UpgradeView(entt::registry &reg)
  : reg{reg} {
  setFixedWidth(200);
  setFixedHeight(100);
}

void UpgradeView::init(SDL_Renderer *ren) {
  font.reset(FC_CreateFont());
  FC_LoadFont(font.get(), ren, res("FreeSans.ttf"), 16, {255, 255, 255, 255}, TTF_STYLE_NORMAL);
}

bool UpgradeView::event(const SDL_Event &e) {
  switch (e.type) {
    case SDL_MOUSEBUTTONDOWN:
      const SDL_Point pos = {e.button.x - viewport().x, e.button.y - viewport().y};
      const SDL_Rect button = {0, 0, 200, 28};
      if (SDL_PointInRect(&pos, &button)) {
        upgradeMotion(reg);
        return true;
      }
  }
  return false;
}

void UpgradeView::render(SDL_Renderer *ren) {
  UpgradeInfo info;
  if (!motionUpgradeInfo(reg, info)) return;
  const SDL_Rect rect = {0, 0, 200, 28};
  SDL_CHECK(SDL_SetRenderDrawColor(ren, 127, 127, 127, 255));
  SDL_CHECK(SDL_RenderFillRect(ren, &rect));
  if (info.cost == -1) {
    FC_Draw(font.get(), ren, 0.0f, 0.0f,
      "Upgrade thrusters: %d/%d",
      info.level, info.total
    );
  } else {
    FC_Draw(font.get(), ren, 0.0f, 0.0f,
      "Upgrade thrusters: %d/%d (%d)",
      info.level, info.total, info.cost
    );
  }
}
