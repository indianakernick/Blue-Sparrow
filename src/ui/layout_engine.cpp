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

void LayoutEngine::init(SDL_Window *window) {
  // TODO: LayoutEngine should define the minimum size of the window
  SDL_Rect viewport;
  SDL_GetWindowSize(window, &viewport.w, &viewport.h);
  viewport.x = 0;
  viewport.y = 0;
  if (root) root->setViewport(viewport);
}

bool LayoutEngine::event(const SDL_Event &e) {
  if (e.type == SDL_WINDOWEVENT) {
    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
      if (root) {
        root->setViewport({0, 0, e.window.data1, e.window.data2});
      }
    }
  }
  return false;
}

void LayoutEngine::setRoot(LayoutItem *item) {
  root = item;
}

void LayoutEngine::evaluate() {
  if (root) root->evaluate();
}
