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
#include <entt/entity/fwd.hpp>

union SDL_Event;
struct SDL_Rect;

class GameView {
public:
  explicit GameView(entt::registry &);
  
  void init(SDL_Renderer *);
  bool event(const SDL_Event &, SDL_Rect);
  void update(float);
  void render(SDL_Renderer *, SDL_Rect);

private:
  entt::registry &reg;
  SDL::Texture foreground;
  SDL::Texture background;
};

#endif
