//
//  map_view.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/3/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_map_view_hpp
#define ui_map_view_hpp

#include "view.hpp"
#include "sdl_delete.hpp"
#include <entt/entity/fwd.hpp>
#include "../comps/graphics.hpp"

class MapView final : public View {
public:
  explicit MapView(entt::registry &);
  
  void init(SDL_Renderer *, FontCache &) override;
  void render(SDL_Renderer *, FontCache &) override;

private:
  entt::registry &reg;
  SDL::Texture minimap;
  Camera camera;
};

#endif
