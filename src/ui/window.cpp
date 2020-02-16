//
//  window.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "window.hpp"

#include <SDL2/SDL_events.h>

Window::Window(SDL_Window *window)
  : window{window} {}

bool Window::event(const SDL_Event &e) {
  do {
    if (e.type != SDL_WINDOWEVENT) break;
    if (e.window.event != SDL_WINDOWEVENT_SIZE_CHANGED) break;
    if (e.window.windowID != SDL_GetWindowID(window)) break;
    setViewport({0, 0, e.window.data1, e.window.data2});
  } while (false);
  return View::event(e);
}

void Window::evaluate() {
  View::evaluate();
  SDL_SetWindowMinimumSize(window, minWidth(), minHeight());
}

void Window::setInitialViewport() {
  SDL_Rect viewport;
  SDL_GetWindowSize(window, &viewport.w, &viewport.h);
  viewport.x = 0;
  viewport.y = 0;
  setViewport(viewport);
}
