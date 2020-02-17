//
//  upgrade_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "upgrade_view.hpp"

#include "font_cache.hpp"
#include "../utils/sdl_check.hpp"
#include <entt/entity/registry.hpp>
#include "../factories/upgrades.hpp"
#include "../utils/resource_path.hpp"

namespace {

using UpgradeFn = bool(entt::registry &);
using GetInfoFn = bool(entt::registry &, UpgradeInfo &);

class UpgradeButton final : public View {
public:
  UpgradeButton(
    entt::registry &reg,
    const char *name,
    UpgradeFn *upgrade,
    GetInfoFn *getInfo
  ) : reg{reg},
      name{name},
      upgrade{upgrade},
      getInfo{getInfo} {
    setFixedWidth(200);
    setFixedHeight(30);
  }
  
  void init(SDL_Renderer *, FontCache &cache) override {
    font = cache.load(res("FreeSans.ttf"), 16);
  }
  
  bool event(const SDL_Event &e) override {
    if (e.type != SDL_MOUSEBUTTONDOWN) return false;
    const SDL_Point pos = {e.button.x - viewport().x, e.button.y - viewport().y};
    const SDL_Rect button = {0, 0, 200, 28};
    if (!SDL_PointInRect(&pos, &button)) return false;
    upgrade(reg);
    return true;
  }
  
  void render(SDL_Renderer *ren, FontCache &cache) override {
    UpgradeInfo info;
    if (!getInfo(reg, info)) return;
    const SDL_Rect rect = {0, 0, 200, 28};
    SDL_CHECK(SDL_SetRenderDrawColor(ren, 127, 127, 127, 255));
    SDL_CHECK(SDL_RenderFillRect(ren, &rect));
    if (info.cost == -1) {
      cache.draw(font, 0.0f, 0.0f,
        "Upgrade %s: %d/%d",
        name, info.level, info.total
      );
    } else {
      cache.draw(font, 0.0f, 0.0f,
        "Upgrade %s: %d/%d (%d)",
        name, info.level, info.total, info.cost
      );
    }
  }
  
private:
  entt::registry &reg;
  const char *name;
  UpgradeFn *upgrade;
  GetInfoFn *getInfo;
  FC_Font *font;
};

}

UpgradeView::UpgradeView(entt::registry &reg) {
  setFixedWidth(200);
  setFixedHeight(120);
  addChild(std::make_unique<UpgradeButton>(
    reg, "thrusters", &upgradeMotion, &motionUpgradeInfo
  ));
  addChild(std::make_unique<UpgradeButton>(
    reg, "blasters", &upgradeBlaster, &blasterUpgradeInfo
  ));
  addChild(std::make_unique<UpgradeButton>(
    reg, "missiles", &upgradeMissile, &missileUpgradeInfo
  ));
  addChild(std::make_unique<UpgradeButton>(
    reg, "hull", &upgradeHull, &hullUpgradeInfo
  ));
}
