//
//  game_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "game_view.hpp"

#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <entt/entity/registry.hpp>

#include "../utils/sdl_check.hpp"
#include "../utils/load_texture.hpp"
#include "../utils/resource_path.hpp"

#include "../systems/all.hpp"
#include "../comps/timers.hpp"
#include "../comps/graphics.hpp"
#include "../systems/camera.hpp"
#include "../systems/render.hpp"
#include "../factories/maps.hpp"
#include "../systems/physics.hpp"
#include "../systems/handle_input.hpp"

namespace {

SDL::Texture makeTexture(SDL_Renderer *renderer) {
  SDL::Texture tex{SDL_CHECK(SDL_CreateTexture(
    renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STATIC, 1, 1
  ))};
  std::uint32_t pixels[] = {0xFFFFFFFF};
  SDL_CHECK(SDL_UpdateTexture(tex.get(), nullptr, pixels, 4));
  return tex;
}

}

GameView::GameView(entt::registry &reg)
  : reg{reg}, camera{reg.set<Camera>()} {
  setWidth({100, 1, 1});
  setHeight({100, 1, 1});
}

void GameView::init(SDL_Renderer *ren, FontCache &cache) {
  foreground = makeTexture(ren);
  background = loadTexture(ren, res("stars_dark.png"));
  
  reg.set<Now>(SDL_GetTicks());
  initializePhysics(reg);
  MapInfo info = makeMap0(reg);
  initializeCamera(camera, info.width, info.height);
  reg.set<MapImage>(std::move(info.image));
  
  View::init(ren, cache);
}

bool GameView::event(const SDL_Event &e) {
  switch (e.type) {
    case SDL_KEYDOWN:
      if (e.key.repeat == 0) {
        return handleKeyDown(reg, e.key.keysym.scancode);
      }
      return false;
    case SDL_KEYUP:
      if (e.key.repeat == 0) {
        return handleKeyUp(reg, e.key.keysym.scancode);
      }
      return false;
    
    case SDL_MOUSEMOTION:
      return handleMouseMove(reg, e.motion.x - viewport().x, e.motion.y - viewport().y);
    case SDL_MOUSEBUTTONDOWN:
      return handleMouseDown(reg, e.button.button);
    case SDL_MOUSEBUTTONUP:
      return handleMouseUp(reg, e.button.button);
      
    default:
      return false;
  }
}

void GameView::update(const float delta) {
  reg.ctx<Now>().time = SDL_GetTicks();
  // TODO: Does this belong here?
  prePhysicsSystems(reg);
  stepPhysics(reg, delta);
  postPhysicsSystems(reg);
}

void GameView::render(SDL_Renderer *ren, FontCache &cache) {
  cameraSystems(reg, camera, viewport());
  renderBackground(reg, {ren, background.get()});
  renderSprite(reg, {ren, foreground.get()});
  renderBar(reg, {ren, nullptr});
  View::render(ren, cache);
}
