//
//  application.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "application.hpp"

#include <SDL2/SDL.h>
#include "game_view.hpp"
#include "box_layout.hpp"
#include "stats_view.hpp"
#include "layout_engine.hpp"
#include "../utils/frame_cap.hpp"
#include "../utils/sdl_check.hpp"
#include "../utils/sdl_delete.hpp"
#include <entt/entity/registry.hpp>

namespace {

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
  // Batching is disabled if we ask for a particular render driver
  SDL_SetHint(SDL_HINT_RENDER_BATCHING, "1");
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

}

Application::Application() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
}

Application::~Application() {
  SDL_Quit();
}

void Application::run() {
  SDL::Window window = initWindow(1280, 720);
  SDL::Renderer renderer = initRenderer(window.get());
  
  entt::registry reg;
  LayoutEngine layout;
  GameView game{reg};
  StatsView stats{reg};
  
  game.init(renderer.get());
  stats.init(renderer.get());
  
  VertBoxLayout panel;
  panel.append(&stats);
  
  HoriBoxLayout root;
  root.append(&panel);
  root.append(&game);
  
  layout.setRoot(&root);
  layout.evaluate();
  layout.init(window.get());

  // TODO: The window could move to a different monitor with a different refresh
  // rate and mess everything up.
  // There doesn't seem to be an event for changing monitors.
  // Might need to check for monitor change on the window move event.
  const int fps = getFPS(window.get());
  FrameCap frame{fps};
  
  while (true) {
    frame.begin();
    
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) return;
      if (layout.event(e)) continue;
      if (stats.event(e)) continue;
      if (game.event(e)) continue;
    }
    
    game.update(1.0f / fps);
    
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), nullptr));
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    
    const SDL_Rect statsViewport = stats.viewport();
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &statsViewport));
    stats.render(renderer.get());
    
    const SDL_Rect gameViewport = game.viewport();
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &gameViewport));
    game.render(renderer.get());
    
    SDL_RenderPresent(renderer.get());
    
    frame.end();
  }
}
