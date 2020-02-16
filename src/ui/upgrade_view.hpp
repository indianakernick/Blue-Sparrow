//
//  upgrade_view.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_upgrade_view_hpp
#define ui_upgrade_view_hpp

#include "sdl_delete.hpp"
#include "layout_item.hpp"
#include <entt/entity/fwd.hpp>

union SDL_Event;

class UpgradeView final : public LayoutItem {
public:
  explicit UpgradeView(entt::registry &);
  
  void init(SDL_Renderer *);
  bool event(const SDL_Event &);
  void render(SDL_Renderer *);

private:
  entt::registry &reg;
  SDL::Font font;
};

#endif
