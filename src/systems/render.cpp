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
#include "../comps/input.hpp"
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

namespace {

void renderRect(
  const Drawing draw,
  const SpriteRect &rect,
  const Sprite sprite,
  const float offX,
  const float offY
) {
  const SDL_Rect srcrect = {0, 0, 1, 1};
  const SDL_FRect dstrect = {rect.x, rect.y, rect.width, rect.height};
  const SDL_FPoint origin = {offX, offY};
  SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, sprite.r, sprite.g, sprite.b));
  SDL_CHECK(SDL_RenderCopyExF(
    draw.ren, draw.fgTex, &srcrect, &dstrect, rect.angle, &origin, SDL_FLIP_NONE
  ));
}

}

void renderSprite(entt::registry &reg) {
  auto draw = reg.ctx<Drawing>();
  entt::each(reg, [&](entt::entity e, SpriteRect rect, Sprite sprite) {
    renderRect(draw, rect, sprite, rect.width / 2.0f, rect.height / 2.0f);
    
    if (auto *input = reg.try_get<MoveInput>(e)) {
      const Sprite thrustSprite = {255, 109, 0};
      
      if (input->forward) {
        const float width = rect.width / 4.0f;
        const float height = rect.height / 3.0f;
        const SpriteRect thrustRect = {
          rect.x - width,
          rect.y + (rect.height - height) / 2.0f,
          width,
          height,
          rect.angle
        };
        const float offX = width + rect.width / 2.0f;
        const float offY = height / 2.0f;
        renderRect(draw, thrustRect, thrustSprite, offX, offY);
      }
      
      if (input->reverse) {
        const float width = rect.width / 5.0f;
        const float height = rect.height / 4.0f;
        const SpriteRect thrustRect = {
          rect.x + rect.width,
          rect.y + (rect.height - height) / 2.0f,
          width,
          height,
          rect.angle
        };
        const float offX = -rect.width / 2.0f;
        const float offY = height / 2.0f;
        renderRect(draw, thrustRect, thrustSprite, offX, offY);
      }
    }
  });
  SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, 255, 255, 255));
}
