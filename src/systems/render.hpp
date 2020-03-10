//
//  render.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_render_hpp
#define systems_render_hpp

#include <entt/entity/fwd.hpp>
#include "../comps/graphics.hpp"

/// Render the background texture
void renderBackground(entt::registry &, DrawCtx);

/// Read SpriteRect and Sprite to render the texture
void renderSprite(entt::registry &, DrawCtx);

/// Read BarRect to render the hull bar
void renderBar(entt::registry &, DrawCtx);

/// Render the map
void renderMap(entt::registry &, DrawCtx);

#endif
