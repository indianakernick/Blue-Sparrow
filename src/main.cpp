//
//  main.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include <SDL2/SDL.h>
#include <box2d/b2_world.h>
#include <entt/entity/registry.hpp>

#include "utils/sdl_check.hpp"
#include "factories/enemy.hpp"
#include "factories/player.hpp"
#include "utils/sdl_delete.hpp"

#include "systems/render.hpp"
#include "systems/expire.hpp"
#include "systems/behave.hpp"
#include "systems/physics.hpp"
#include "systems/find target.hpp"
#include "systems/apply input.hpp"
#include "systems/handle input.hpp"
#include "systems/read physics.hpp"

SDL::Texture makeTexture(SDL_Renderer *renderer) {
  SDL::Texture tex{SDL_CHECK(SDL_CreateTexture(
    renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STATIC, 1, 1
  ))};
  std::uint32_t pixels[] = {0xFFFFFFFF};
  SDL_CHECK(SDL_UpdateTexture(tex.get(), nullptr, pixels, 4));
  return tex;
}

int main() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
  SDL::Window window{SDL_CHECK(SDL_CreateWindow(
    "Blue Sparrow",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 720,
    SDL_WINDOW_SHOWN
  ))};
  
  // THIS DOESN'T DO ANYTHING !!!!!!!!!!!!!!!
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  SDL::Renderer renderer{SDL_CHECK(SDL_CreateRenderer(
    window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ))};
  SDL::Texture texture = makeTexture(renderer.get());
  
  entt::registry reg;
  connectDestroyBody(reg);
  reg.set<b2World>(b2Vec2{0.0f, 0.0f});
  reg.set<SDL_Renderer *>(renderer.get());
  reg.set<SDL_Texture *>(texture.get());
  setTransform(reg, makePlayer(reg), {32.0f, 36.0f}, 0.0f);
  setTransform(reg, makeEnemy(reg), {96.0f, 36.0f}, b2_pi);
  
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        break;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.repeat != 0) continue;
        handleKeyDown(reg, e.key.keysym.scancode);
      } else if (e.type == SDL_KEYUP) {
        if (e.key.repeat != 0) continue;
        handleKeyUp(reg, e.key.keysym.scancode);
      }
    }
    
    findTarget(reg);
    behaveBasic(reg);
    applyMoveInput(reg);
    applyBlasterInput(reg);
    expireTemporary(reg);
    stepPhysics(reg);
    readPhysicsTransform(reg);
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    
    renderSprite(reg);
    
    SDL_RenderPresent(renderer.get());
  }
  
  SDL_Quit();
}
