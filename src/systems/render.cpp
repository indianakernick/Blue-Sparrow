//
//  render.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "render.hpp"

#include "sdl_check.hpp"
#include <SDL2/SDL_render.h>
#include "../utils/each.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

void renderBackground(entt::registry &reg) {
  auto draw = reg.ctx<Drawing>();
  auto cam = reg.ctx<Camera>();
  entt::each(reg, [=](SpriteRect rect, BackgroundSprite) {
    const SDL_Rect srcrect = {0, 0, int(cam.arenaWidth * cam.maxZoom), int(cam.arenaHeight * cam.maxZoom)};
    const SDL_FRect dstrect = {rect.x, rect.y, rect.width, rect.height};
    SDL_CHECK(SDL_RenderCopyF(draw.ren, draw.bgTex, &srcrect, &dstrect));
  });
}

void renderSprite(entt::registry &reg) {
  auto draw = reg.ctx<Drawing>();
  entt::each(reg, [=](SpriteRect rect, Sprite sprite) {
    const SDL_Rect srcrect = {0, 0, 1, 1};
    const SDL_FRect dstrect = {rect.x, rect.y, rect.width, rect.height};
    SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, sprite.r, sprite.g, sprite.b));
    SDL_CHECK(SDL_RenderCopyExF(
      draw.ren, draw.fgTex, &srcrect, &dstrect, rect.angle, nullptr, SDL_FLIP_NONE
    ));
  });
  SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, 255, 255, 255));
}
