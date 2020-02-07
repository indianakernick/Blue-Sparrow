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

b2Body *makeShip(b2World &world) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.angularDamping = 10.0;
  bodyDef.linearDamping = 0.2;
  bodyDef.position = {64, 36};
  bodyDef.angle = 0;
  
  b2PolygonShape shape;
  shape.SetAsBox(1.5, 1);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 0.1;
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return body;
}

SDL::Texture makeTexture(SDL_Renderer *renderer) {
  SDL::Texture tex{SDL_CHECK(SDL_CreateTexture(
    renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STATIC, 1, 1
  ))};
  uint32_t pixels[] = {0x0000FFFF};
  SDL_CHECK(SDL_UpdateTexture(tex.get(), nullptr, pixels, 4));
  return tex;
}

int main() {
  b2World world{{0, 0}};
  b2Body *ship = makeShip(world);

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
  
  bool forward = false;
  bool left = false;
  bool right = false;
  bool running = true;
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = false;
        break;
      } else if (e.type == SDL_KEYDOWN) {
        if (e.key.repeat != 0) continue;
        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_UP:
            forward = true;
            break;
          case SDL_SCANCODE_LEFT:
            left = true;
            break;
          case SDL_SCANCODE_RIGHT:
            right = true;
            break;
          default: ;
        }
      } else if (e.type == SDL_KEYUP) {
        if (e.key.repeat != 0) continue;
        switch (e.key.keysym.scancode) {
          case SDL_SCANCODE_UP:
            forward = false;
            break;
          case SDL_SCANCODE_LEFT:
            left = false;
            break;
          case SDL_SCANCODE_RIGHT:
            right = false;
            break;
          default: ;
        }
      } else if (e.type == SDL_WINDOWEVENT_FOCUS_LOST) {
        forward = false;
        left = false;
        right = false;
      }
    }
    
    if (forward) {
      const float angle = ship->GetAngle();
      const float mag = 10.0f;
      const b2Vec2 force {std::cos(angle) * mag, std::sin(angle) * mag};
      ship->ApplyForceToCenter(force, true);
    }
    if (left && !right) {
      ship->ApplyTorque(-20.0f, true);
    }
    if (right && !left) {
      ship->ApplyTorque(20.0f, true);
    }
    
    world.Step(1.0f/60.0, 8, 4);
    
    SDL_CHECK(SDL_SetRenderDrawColor(renderer.get(), 0, 0, 0, 255));
    SDL_CHECK(SDL_RenderClear(renderer.get()));
    
    const b2Vec2 pos = ship->GetPosition();
    const float angle = ship->GetAngle();
    const SDL_Rect srcrect = {0, 0, 1, 1};
    const SDL_FRect dstrect = {(pos.x - 3) * 10, (pos.y - 2) * 10, 30, 20};
    SDL_CHECK(SDL_RenderCopyExF(
      renderer.get(), texture.get(), &srcrect, &dstrect, angle * 180.0 / 3.14159, nullptr, SDL_FLIP_NONE
    ));
    
    SDL_RenderPresent(renderer.get());
  }
  
  SDL_Quit();
}
