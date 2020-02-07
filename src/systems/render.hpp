//
//  render.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef render_hpp
#define render_hpp

#include <entt/entity/fwd.hpp>

struct SDL_Texture;
struct SDL_Renderer;

void renderSprite(entt::registry &, SDL_Renderer *, SDL_Texture *);

#endif
