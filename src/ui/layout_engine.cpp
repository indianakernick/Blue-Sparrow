//
//  layout_engine.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "layout_engine.hpp"

#include "layout_item.hpp"
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

LayoutEngine::LayoutEngine(SDL_Window *window)
  : window{window} {}

bool LayoutEngine::event(const SDL_Event &e) {
  if (!root) return false;
  if (e.type != SDL_WINDOWEVENT) return false;
  if (e.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) return false;
  if (e.window.windowID != SDL_GetWindowID(window)) return false;
  root->setViewport({0, 0, e.window.data1, e.window.data2});
  return false;
}

void LayoutEngine::setRoot(LayoutItem *item) {
  root = item;
}

void LayoutEngine::evaluate() {
  if (root) {
    root->evaluate();
    SDL_SetWindowMinimumSize(window, root->minWidth(), root->minHeight());
  }
}

void LayoutEngine::setInitialViewport() {
  if (root) {
    SDL_Rect viewport;
    SDL_GetWindowSize(window, &viewport.w, &viewport.h);
    viewport.x = 0;
    viewport.y = 0;
    root->setViewport(viewport);
  }
}
