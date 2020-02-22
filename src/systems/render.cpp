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
  const float originX,
  const float originY
) {
  const SDL_Rect srcrect = {0, 0, 1, 1};
  const SDL_FRect dstrect = {rect.x, rect.y, rect.width, rect.height};
  const SDL_FPoint origin = {originX, originY};
  SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, sprite.r, sprite.g, sprite.b));
  SDL_CHECK(SDL_RenderCopyExF(
    draw.ren, draw.fgTex, &srcrect, &dstrect, rect.angle, &origin, SDL_FLIP_NONE
  ));
}

void renderRelativeRect(
  const Drawing draw,
  const SpriteRect &baseRect,
  const SpriteRect &relRect,
  const Sprite sprite
) {
  const SpriteRect rect = {
    baseRect.x + relRect.x,
    baseRect.y + relRect.y,
    relRect.width,
    relRect.height,
    baseRect.angle
  };
  const float originX = baseRect.width / 2.0f - relRect.x;
  const float originY = baseRect.height / 2.0f - relRect.y;
  renderRect(draw, rect, sprite, originX, originY);
}

}

void renderSprite(entt::registry &reg) {
  auto draw = reg.ctx<Drawing>();
  entt::each(reg, [&](entt::entity e, SpriteRect rect, Sprite sprite) {
    renderRect(draw, rect, sprite, rect.width / 2.0f, rect.height / 2.0f);
    
    if (auto *input = reg.try_get<MoveCommand>(e)) {
      const Sprite thrustSprite = {255, 109, 0};
      
      if (input->forward) {
        const float width = rect.width / 4.0f;
        const float height = rect.height / 3.0f;
        const float x = -width;
        const float y = (rect.height - height) / 2.0f;
        renderRelativeRect(draw, rect, {x, y, width, height, 0}, thrustSprite);
      }
      
      if (input->reverse) {
        const float width = rect.width / 5.0f;
        const float height = rect.height / 4.0f;
        const float x = rect.width;
        const float y = (rect.height - height) / 2.0f;
        renderRelativeRect(draw, rect, {x, y, width, height, 0}, thrustSprite);
      }
      
      {
        const float width = rect.width / 6.0f;
        const float height = width;
        const float x1 = rect.width / 4.0f - width / 2.0f;
        const float x2 = rect.width * 3.0f/4.0f - width / 2.0f;
        const float y1 = -height;
        const float y2 = rect.height;
        
        if (input->ccw && !input->cw) {
          renderRelativeRect(draw, rect, {x1, y1, width, height, 0}, thrustSprite);
          renderRelativeRect(draw, rect, {x2, y2, width, height, 0}, thrustSprite);
        }
        
        if (input->cw && !input->ccw) {
          renderRelativeRect(draw, rect, {x1, y2, width, height, 0}, thrustSprite);
          renderRelativeRect(draw, rect, {x2, y1, width, height, 0}, thrustSprite);
        }
      }
      
      {
        const float width = rect.width / 5.0f;
        const float height = width;
        const float x = (rect.width - width) / 2.0f;
        
        if (input->left) {
          renderRelativeRect(draw, rect, {x, rect.height, width, height, 0}, thrustSprite);
        }
        
        if (input->right) {
          renderRelativeRect(draw, rect, {x, -height, width, height, 0}, thrustSprite);
        }
      }
    }
  });
  SDL_CHECK(SDL_SetTextureColorMod(draw.fgTex, 255, 255, 255));
}

void renderBar(entt::registry &reg) {
  // TODO: Is this faster with three separate loops?
  auto draw = reg.ctx<Drawing>();
  entt::each(reg, [&](const BarRect rect) {
    const int progWidth = rect.width * rect.progress + 0.5f;
    const int antiWidth = rect.width - progWidth;
    const SDL_Rect value = {rect.x, rect.y, progWidth, rect.height};
    const SDL_Rect anti = {rect.x + progWidth, rect.y, antiWidth, rect.height};
    const SDL_Rect outline = {rect.x - 1, rect.y - 1, rect.width + 2, rect.height + 2};
    SDL_CHECK(SDL_SetRenderDrawColor(draw.ren, 0, 255, 0, 255));
    SDL_CHECK(SDL_RenderFillRect(draw.ren, &value));
    SDL_CHECK(SDL_SetRenderDrawColor(draw.ren, 255, 0, 0, 255));
    SDL_CHECK(SDL_RenderFillRect(draw.ren, &anti));
    SDL_CHECK(SDL_SetRenderDrawColor(draw.ren, 255, 255, 255, 255));
    SDL_CHECK(SDL_RenderDrawRect(draw.ren, &outline));
  });
}
