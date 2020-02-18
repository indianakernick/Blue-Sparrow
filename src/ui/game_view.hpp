//
//  game_view.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_game_view_hpp
#define ui_game_view_hpp

#include "view.hpp"
#include "sdl_delete.hpp"
#include <entt/entity/fwd.hpp>

class GameView final : public View {
public:
  explicit GameView(entt::registry &);
  
  void init(SDL_Renderer *, FontCache &) override;
  bool event(const SDL_Event &) override;
  void update(float) override;
  void render(SDL_Renderer *, FontCache &) override;

private:
  entt::registry &reg;
  SDL::Texture foreground;
  SDL::Texture background;
};

#endif
