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

/// Render the background texture
void renderBackground(entt::registry &);

/// Read SpriteRect and Sprite to render the texture
void renderSprite(entt::registry &);

#endif
