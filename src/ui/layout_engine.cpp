//
//  layout_engine.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "layout_engine.hpp"

#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

namespace {

void updateViewport(SDL_Rect &viewport, const SDL_Point windowSize) {
  viewport.w = windowSize.x - viewport.x;
  viewport.h = windowSize.y - viewport.y;
}

}

void LayoutEngine::init(SDL_Window *window) {
  SDL_Point windowSize;
  SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);
  viewport.x = 200;
  viewport.y = 0;
  updateViewport(viewport, windowSize);
}

bool LayoutEngine::event(const SDL_Event &e) {
  if (e.type == SDL_WINDOWEVENT) {
    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      updateViewport(viewport, {e.window.data1, e.window.data2});
    }
  }
  return false;
}

SDL_Rect LayoutEngine::gameViewport() const {
  return viewport;
}

SDL_Rect LayoutEngine::statsViewport() const {
  return {0, 0, viewport.x, viewport.h};
}
