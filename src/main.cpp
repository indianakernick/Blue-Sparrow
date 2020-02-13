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

#include "comps/graphics.hpp"
#include "factories/ships.hpp"
#include "factories/arena.hpp"
#include "utils/sdl_check.hpp"
#include "utils/sdl_delete.hpp"
#include "utils/load_texture.hpp"

#include "systems/camera.hpp"
#include "systems/render.hpp"
#include "systems/expire.hpp"
#include "systems/behave.hpp"
#include "systems/physics.hpp"
#include "systems/collisions.hpp"
#include "systems/find_target.hpp"
#include "systems/handle_input.hpp"
#include "systems/apply_commands.hpp"

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

void setMaxZoom(SDL_Texture *tex, Camera &cam) {
  int width, height;
  SDL_CHECK(SDL_QueryTexture(tex, nullptr, nullptr, &width, &height));
  cam.maxZoom = std::min(width / cam.arenaWidth, height / cam.arenaHeight);
}

void setMinZoom(Camera &cam) {
  cam.minZoom = std::max(cam.width / cam.arenaWidth, cam.height / cam.arenaHeight);
}

int main() {
  SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
  
  SDL::Window window{SDL_CHECK(SDL_CreateWindow(
    "Blue Sparrow",
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    1280, 720,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
  ))};
  
  // THIS DOESN'T DO ANYTHING !!!!!!!!!!!!!!!
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  // Workaround for crash when exiting fullscreen
  SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
  SDL::Renderer renderer{SDL_CHECK(SDL_CreateRenderer(
    window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  ))};
  SDL::Texture foreground = makeTexture(renderer.get());
  SDL::Texture background = loadTexture(renderer.get(), res("stars.png").c_str());
  
  entt::registry reg;
  reg.set<b2World>(b2Vec2{0.0f, 0.0f});
  connectDestroyBody(reg);
  connectContactListener(reg);
  
  Drawing drawing;
  drawing.ren = renderer.get();
  drawing.fgTex = foreground.get();
  drawing.bgTex = background.get();
  reg.set<Drawing>(drawing);
  
  Camera camera;
  camera.x = 0.0f;
  camera.y = 0.0f;
  camera.width = 1280;
  camera.height = 720;
  camera.arenaWidth = 150.0f;
  camera.arenaHeight = 150.0f;
  setMaxZoom(background.get(), camera);
  setMinZoom(camera);
  camera.zoom = camera.maxZoom;
  reg.set<Camera>(camera);
  
  setTransform(reg, makePlayer(reg), {0.0f, 0.0f}, 0.0f);
  setTransform(reg, makeSniper(reg, Team::enemy), {20.0f, 0.0f}, b2_pi);
  setTransform(reg, makeScout(reg, Team::enemy), {20.0f, 10.0f}, b2_pi);
  setTransform(reg, makeScout(reg, Team::enemy), {20.0f, -10.0f}, b2_pi);
  setTransform(reg, makeScout(reg, Team::ally), {-20.0f, 0.0f}, 0.0f);
  makeArena(reg, camera.arenaWidth, camera.arenaHeight);
  entt::entity rock = makeAsteroid(reg);
  setTransform(reg, rock, {-30.0f, -40.0f}, 1.0f);
  setMotion(reg, rock, {1.0f, 1.3f}, 0.1f);
  
  // TODO: The window could move to a different monitor with a different refresh
  // rate and mess everything up.
  // There doesn't seem to be an event for changing monitors.
  // Might need to check for monitor change on the window move event.
  const int display = SDL_CHECK(SDL_GetWindowDisplayIndex(window.get()));
  SDL_DisplayMode displayMode;
  SDL_CHECK(SDL_GetCurrentDisplayMode(display, &displayMode));
  const int fps = displayMode.refresh_rate == 0 ? 60 : displayMode.refresh_rate;
  
  const std::uint64_t lowerFrameTime = SDL_GetPerformanceFrequency() / (fps + fps / 2);
  const std::uint64_t targetFrameTime = SDL_GetPerformanceFrequency() / fps;
  const std::uint64_t toMilli = SDL_GetPerformanceFrequency() / 1000;
  
  while (true) {
    const std::uint64_t frameStart = SDL_GetPerformanceCounter();
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
            Camera &cam = reg.ctx<Camera>();
            cam.width = e.window.data1;
            cam.height = e.window.data2;
            setMinZoom(cam);
            cam.zoom = std::max(cam.zoom, cam.minZoom);
          }
          break;
        case SDL_MOUSEMOTION:
          handleMouseMove(reg, e.motion.x, e.motion.y);
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
    
    /*
    const float top = camera.maxZoom;
    const float bottom = camera.minZoom;
    const float period = 10000.0f;
    const float radians = (2.0f * b2_pi * SDL_GetTicks()) / period;
    const float scale = (top - bottom) / 2.0;
    reg.ctx<Camera>().zoom = (std::cos(radians) + 1.0f) * scale + bottom;
    */
    
    findTarget(reg);
    behaveOrbit(reg);
    behaveSeek(reg);
    behaveSniper(reg);
    behaveMouse(reg);
    applyMoveCommands(reg);
    applyBlasterCommands(reg);
    applyMissileCommands(reg);
    expireTemporary(reg);
    
    stepPhysics(reg, fps);
    
    limitVelocity(reg);
    handleCollisions(reg);
    moveCamera(reg);
    writeSpriteRect(reg);
    writeBarRect(reg);
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    
    renderBackground(reg);
    renderSprite(reg);
    renderBar(reg);
    
    SDL_RenderPresent(renderer.get());
    
    const std::uint64_t frameEnd = SDL_GetPerformanceCounter();
    const std::uint64_t frameLen = frameEnd - frameStart;
    if (frameLen < lowerFrameTime) {
      const std::uint64_t delayTime = (targetFrameTime - frameLen) / toMilli;
      SDL_Delay(static_cast<std::uint32_t>(delayTime));
    }
  }
  
  SDL_Quit();
}
