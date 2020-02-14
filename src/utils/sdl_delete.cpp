//
//  sdl_delete.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "sdl_delete.hpp"

#include <SDL_FontCache.h>
#include <SDL2/SDL_render.h>

void SDL::DeleteFont::operator()(FC_Font *ptr) const noexcept {
  FC_FreeFont(ptr);
}

void SDL::DeleteWindow::operator()(SDL_Window *ptr) const noexcept {
  SDL_DestroyWindow(ptr);
}

void SDL::DeleteTexture::operator()(SDL_Texture *ptr) const noexcept {
  SDL_DestroyTexture(ptr);
}

void SDL::DeleteSurface::operator()(SDL_Surface *ptr) const noexcept {
  SDL_FreeSurface(ptr);
}

void SDL::DeleteRenderer::operator()(SDL_Renderer *ptr) const noexcept {
  SDL_DestroyRenderer(ptr);
}
