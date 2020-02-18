//
//  application.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "application.hpp"

#include <SDL2/SDL.h>
#include "window.hpp"
#include "game_view.hpp"
#include "font_cache.hpp"
#include "stats_view.hpp"
#include "upgrade_view.hpp"
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

// operator!
// operator+

// can we use a dirty flag to evaluate and set the viewport?
// setting the size would mark the item as dirty
// calling evaluate would mark the item as clean
//   how can parents know that their grandchildren are dirty without checking?

// Whenever a child is added or resized, the parent must be updated
// If the parent must be resized, it's parent must be updated
//   we get a lot of unnecessary computation during initialization
//   if we're careful with initialization order, can we avoid that?
//   initialize from the bottom up
//   if an item doesn't have a parent, it doesn't need to update it

// Whenever the viewport is requested
// Update it if required
// Somehow use dirty flag

void Application::run() {
  SDL::Window window = initWindow(1280, 720);
  SDL::Renderer renderer = initRenderer(window.get());
  FontCache cache{renderer.get()};
  entt::registry reg;
  
  auto panel = std::make_unique<View>();
  panel->setLayout({LayoutDir::down});
  panel->addChild(std::make_unique<StatsView>(reg));
  panel->addChild(std::make_unique<UpgradeView>(reg));
  
  Window root{window.get()};
  root.setLayout({LayoutDir::right});
  root.addChild(std::move(panel));
  root.addChild(std::make_unique<GameView>(reg));
  
  root.setInitialViewport();
  root.init(renderer.get(), cache);

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
      root.event(e);
    }
    
    root.update(1.0f / fps);
    
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), nullptr));
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    root.render(renderer.get(), cache);
    SDL_RenderPresent(renderer.get());
    
    frame.end();
  }
}
