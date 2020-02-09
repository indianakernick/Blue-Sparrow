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

void moveCamera(entt::registry &reg) {
  auto &cam = reg.ctx<Camera>();
  entt::each(reg, [&](SpriteRect rect, CameraFocus) {
    cam.centerX = rect.x + rect.width / 2.0f;
    cam.centerY = rect.y + rect.height / 2.0f;
    cam.centerX = std::max(cam.centerX, cam.width / 2.0f - cam.arenaWidth / 2.0f);
    cam.centerX = std::min(cam.centerX, cam.arenaWidth / 2.0f - cam.width / 2.0f);
    cam.centerY = std::max(cam.centerY, cam.height / 2.0f - cam.arenaHeight / 2.0f);
    cam.centerY = std::min(cam.centerY, cam.arenaHeight / 2.0f - cam.height / 2.0f);
  });
}

void renderBackground(entt::registry &reg) {
  auto draw = reg.ctx<Drawing>();
  auto cam = reg.ctx<Camera>();
  
  const int x = cam.centerX - cam.width / 2.0f + cam.arenaWidth / 2.0f;
  const int y = cam.centerY - cam.height / 2.0f + cam.arenaHeight / 2.0f;
  const SDL_Rect srcrect = {x, y, cam.width, cam.height};
  const SDL_Rect dstrect = {0, 0, cam.width, cam.height};
  
  SDL_CHECK(SDL_RenderCopy(draw.ren, draw.bgTex, &srcrect, &dstrect));
}

void renderSprite(entt::registry &reg) {
  auto draw = reg.ctx<Drawing>();
  auto cam = reg.ctx<Camera>();
  entt::each(reg, [=](SpriteRect rect, Sprite sprite) {
    const SDL_Rect srcrect = {0, 0, 1, 1};
    const float x = rect.x - cam.centerX + cam.width / 2.0f;
    const float y = rect.y - cam.centerY + cam.height / 2.0f;
    const SDL_FRect dstrect = {x, y, rect.width, rect.height};
    SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, sprite.r, sprite.g, sprite.b));
    SDL_CHECK(SDL_RenderCopyExF(
      draw.ren, draw.fgTex, &srcrect, &dstrect, rect.angle, nullptr, SDL_FLIP_NONE
    ));
  });
  SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, 255, 255, 255));
  
  // Just for debugging
  /*
  const SDL_FRect arena = {
    -cam.arenaWidth / 2.0f - cam.centerX + cam.width / 2.0f,
    -cam.arenaHeight / 2.0f - cam.centerY + cam.height / 2.0f,
    static_cast<float>(cam.arenaWidth),
    static_cast<float>(cam.arenaHeight)
  };
  SDL_CHECK(SDL_SetRenderDrawColor(draw.ren, 255, 255, 255, 255));
  SDL_CHECK(SDL_RenderDrawRectF(draw.ren, &arena));
  */
}
