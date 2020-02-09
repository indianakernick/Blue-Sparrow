//
//  handle_input.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_handle_input_hpp
#define systems_handle_input_hpp

#include <SDL2/SDL_scancode.h>
#include <entt/entity/fwd.hpp>

/// Respond to key-down events by setting MoveInput when KeyInput is present
bool handleKeyDown(entt::registry &, SDL_Scancode);
/// Respond to key-up events by setting MoveInput when KeyInput is present
bool handleKeyUp(entt::registry &, SDL_Scancode);

#endif
