//
//  game_view.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_game_view_hpp
#define ui_game_view_hpp

#include "sdl_delete.hpp"
#include "layout_item.hpp"
#include <entt/entity/fwd.hpp>

union SDL_Event;
struct SDL_Rect;

class GameView : public LayoutItem {
public:
  explicit GameView(entt::registry &);
  
  void init(SDL_Renderer *);
  bool event(const SDL_Event &);
  void update(float);
  void render(SDL_Renderer *);

private:
  entt::registry &reg;
  SDL::Texture foreground;
  SDL::Texture background;
};

#endif