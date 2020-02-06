//
//  main.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <entt/entity/registry.hpp>

#include "utils/sdl_check.hpp"
#include "utils/sdl_delete.hpp"

int main() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
  SDL::Window window{SDL_CHECK(SDL_CreateWindow(
    "Blue Sparrow",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 720,
    SDL_WINDOW_SHOWN
  ))};
  
  SDL::Renderer renderer{SDL_CHECK(SDL_CreateRenderer(
    window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ))};
  
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        break;
      }
    }
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 63, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    SDL_RenderPresent(renderer.get());
  }
  
  SDL_Quit();
}
