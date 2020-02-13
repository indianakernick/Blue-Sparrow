//
//  main.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include <string>
#include <SDL2/SDL.h>
#include <box2d/b2_world.h>
#include <entt/entity/registry.hpp>

#include "systems/all.hpp"
#include "comps/graphics.hpp"
#include "systems/camera.hpp"
#include "systems/physics.hpp"
#include "factories/ships.hpp"
#include "factories/arena.hpp"
#include "utils/frame_cap.hpp"
#include "utils/sdl_check.hpp"
#include "utils/sdl_delete.hpp"
#include "utils/load_texture.hpp"
#include "systems/handle_input.hpp"

SDL::Texture makeTexture(SDL_Renderer *renderer) {
  SDL::Texture tex{SDL_CHECK(SDL_CreateTexture(
    renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STATIC, 1, 1
  ))};
  std::uint32_t pixels[] = {0xFFFFFFFF};
  SDL_CHECK(SDL_UpdateTexture(tex.get(), nullptr, pixels, 4));
  return tex;
}

std::string res(const char *path) {
  return std::string(SDL_CHECK(SDL_GetBasePath())) + path;
}

SDL::Window initializeWindow(const int width, const int height) {
  return SDL::Window{SDL_CHECK(SDL_CreateWindow(
    "Blue Sparrow",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    width, height,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  ))};
}

SDL::Renderer initializeRenderer(SDL_Window *window) {
  // THIS DOESN'T DO ANYTHING !!!!!!!!!!!!!!!
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  // Workaround for crash when exiting fullscreen
  // Also a workaround for a possibly related issue with multiple viewports
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  return SDL::Renderer{SDL_CHECK(SDL_CreateRenderer(
    window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ))};
}

void updateViewport(SDL_Rect &viewport, const SDL_Point windowSize) {
  viewport.w = windowSize.x - viewport.x;
  viewport.h = windowSize.y - viewport.y;
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

int getFPS(SDL_Window *window) {
  const int display = SDL_CHECK(SDL_GetWindowDisplayIndex(window));
  SDL_DisplayMode displayMode;
  SDL_CHECK(SDL_GetCurrentDisplayMode(display, &displayMode));
  return displayMode.refresh_rate == 0 ? 60 : displayMode.refresh_rate;
}

int main() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
  SDL::Window window = initializeWindow(1280, 720);
  SDL::Renderer renderer = initializeRenderer(window.get());
  SDL::Texture foreground = makeTexture(renderer.get());
  SDL::Texture background = loadTexture(renderer.get(), res("stars.png").c_str());

  SDL_Rect viewport = {100, 0, 0, 0};
  entt::registry reg;
  initializePhysics(reg);
  initializeWorld(reg, 150.0f);
  initializeCamera(reg, 150.0f);

  SDL_Point bgSize;
  SDL_CHECK(SDL_QueryTexture(background.get(), nullptr, nullptr, &bgSize.x, &bgSize.y));
  updateCameraBackground(reg, bgSize);
  
  SDL_Point windowSize;
  SDL_CHECK(SDL_GetRendererOutputSize(renderer.get(), &windowSize.x, &windowSize.y));
  updateViewport(viewport, windowSize);
  updateCameraViewport(reg, viewport);
  
  Drawing drawing;
  drawing.ren = renderer.get();
  drawing.fgTex = foreground.get();
  drawing.bgTex = background.get();
  reg.set<Drawing>(drawing);
  
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
      switch (e.type) {
        case SDL_QUIT:
          quit = true;
          break;
        case SDL_KEYDOWN:
          if (e.key.repeat == 0) {
            handleKeyDown(reg, e.key.keysym.scancode);
          }
          break;
        case SDL_KEYUP:
          if (e.key.repeat == 0) {
            handleKeyUp(reg, e.key.keysym.scancode);
          }
          break;
        case SDL_WINDOWEVENT:
          if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_Point windowSize;
            SDL_CHECK(SDL_GetRendererOutputSize(renderer.get(), &windowSize.x, &windowSize.y));
            updateViewport(viewport, windowSize);
            updateCameraViewport(reg, viewport);
          }
          break;
        case SDL_MOUSEMOTION:
          handleMouseMove(reg, e.motion.x - viewport.x, e.motion.y - viewport.y);
          break;
        case SDL_MOUSEBUTTONDOWN:
          handleMouseDown(reg, e.button.button);
          break;
        case SDL_MOUSEBUTTONUP:
          handleMouseUp(reg, e.button.button);
          break;
          
        default: ;
      }
      if (quit) break;
    }
    if (quit) break;
    
    prePhysicsSystems(reg);
    stepPhysics(reg, fps);
    postPhysicsSystems(reg);
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    
    SDL_CHECK(SDL_RenderSetViewport(renderer.get(), &viewport));
    renderSystems(reg);
    
    SDL_RenderPresent(renderer.get());
    frame.end();
  }
  
  SDL_Quit();
}
