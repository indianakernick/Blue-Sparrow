//
//  game_view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "game_view.hpp"

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>
#include <entt/entity/registry.hpp>

#include "../utils/sdl_check.hpp"
#include "../utils/load_texture.hpp"
#include "../utils/resource_path.hpp"

#include "../systems/all.hpp"
#include "../comps/graphics.hpp"
#include "../systems/camera.hpp"
#include "../systems/physics.hpp"
#include "../factories/ships.hpp"
#include "../factories/arena.hpp"
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

void initializeWorld(entt::registry &reg, const float arenaSize) {
  setTransform(reg, makePlayer(reg), {0.0f, 0.0f}, 0.0f);
  setTransform(reg, makeSniper(reg, Team::enemy), {20.0f, 0.0f}, b2_pi);
  setTransform(reg, makeScout(reg, Team::enemy), {20.0f, 10.0f}, b2_pi);
  setTransform(reg, makeScout(reg, Team::enemy), {20.0f, -10.0f}, b2_pi);
  setTransform(reg, makeScout(reg, Team::ally), {-20.0f, 0.0f}, 0.0f);
  makeArena(reg, arenaSize, arenaSize);
  entt::entity rock = makeAsteroid(reg);
  setTransform(reg, rock, {-30.0f, -40.0f}, 1.0f);
  setMotion(reg, rock, {1.0f, 1.3f}, 0.1f);
}

}

GameView::GameView(entt::registry &reg)
  : reg{reg} {
  setGrowWidth(100);
  setGrowHeight(100);
}

void GameView::init(SDL_Renderer *ren) {
  foreground = makeTexture(ren);
  background = loadTexture(ren, res("stars.png"));
  
  {
    Drawing drawing;
    drawing.ren = ren;
    drawing.fgTex = foreground.get();
    drawing.bgTex = background.get();
    reg.set<Drawing>(drawing);
  }
  
  initializePhysics(reg);
  initializeWorld(reg, 150.0f);
  initializeCamera(reg, 150.0f);
  
  {
    SDL_Point bgSize;
    SDL_CHECK(SDL_QueryTexture(background.get(), nullptr, nullptr, &bgSize.x, &bgSize.y));
    updateCameraBackground(reg, bgSize);
  }
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
  prePhysicsSystems(reg);
  stepPhysics(reg, delta);
  postPhysicsSystems(reg);
}

void GameView::render(SDL_Renderer *) {
  cameraSystems(reg, viewport());
  renderSystems(reg);
}
