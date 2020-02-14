//
//  main.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include <SDL2/SDL.h>
#include "ui/game_view.hpp"
#include "ui/stats_view.hpp"
#include "ui/layout_engine.hpp"
#include "utils/frame_cap.hpp"
#include "utils/sdl_check.hpp"
#include "utils/sdl_delete.hpp"
#include <entt/entity/registry.hpp>

SDL::Window initWindow(const int width, const int height) {
  return SDL::Window{SDL_CHECK(SDL_CreateWindow(
    "Blue Sparrow",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width, height,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  ))};
}

SDL::Renderer initRenderer(SDL_Window *window) {
  // THIS DOESN'T DO ANYTHING !!!!!!!!!!!!!!!
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  // Workaround for crash when exiting fullscreen
  // Also a workaround for a possibly related issue with multiple viewports
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  return SDL::Renderer{SDL_CHECK(SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ))};
}

int getFPS(SDL_Window *window) {
  const int display = SDL_CHECK(SDL_GetWindowDisplayIndex(window));
  SDL_DisplayMode displayMode;
  SDL_CHECK(SDL_GetCurrentDisplayMode(display, &displayMode));
  return displayMode.refresh_rate == 0 ? 60 : displayMode.refresh_rate;
}

int main() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
  SDL::Window window = initWindow(1280, 720);
  SDL::Renderer renderer = initRenderer(window.get());
  
  entt::registry reg;
  LayoutEngine layout;
  GameView game{reg};
  StatsView stats{reg};
  
  layout.init(window.get());
  game.init(renderer.get());
  stats.init(renderer.get());
  
  // TODO: The window could move to a different monitor with a different refresh
  // rate and mess everything up.
  // There doesn't seem to be an event for changing monitors.
  // Might need to check for monitor change on the window move event.
  const int fps = getFPS(window.get());
  FrameCap frame{fps};
  
  while (true) {
    frame.begin();
    SDL_Event e;
    bool quit = false;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
        break;
      }
      if (layout.event(e)) continue;
      if (stats.event(e, layout.statsViewport())) continue;
      if (game.event(e, layout.gameViewport())) continue;
    }
    if (quit) break;
    
    game.update(fps);
    
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), nullptr));
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    
    const SDL_Rect statsViewport = layout.statsViewport();
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &statsViewport));
    stats.render(renderer.get(), statsViewport);
    
    const SDL_Rect gameViewport = layout.gameViewport();
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &gameViewport));
    game.render(renderer.get(), gameViewport);
    
    SDL_RenderPresent(renderer.get());
    frame.end();
  }
  
  SDL_Quit();
}
