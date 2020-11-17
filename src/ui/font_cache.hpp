//
//  font_cache.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 17/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_font_cache_hpp
#define ui_font_cache_hpp

#include <vector>
#include <SDL_pixels.h>
#include <SDL_FontCache.h>
#include "../utils/sdl_delete.hpp"

struct SDL_Renderer;
struct SDL_Color;

class FontCache {
public:
  explicit FontCache(SDL_Renderer *);

  FC_Font *load(const char *, int, SDL_Color = {255, 255, 255, 255});
  
  template <typename... Args>
  void draw(FC_Font *font, const float x, const float y, Args... args) {
    FC_Draw(font, ren, x, y, args...);
  }

private:
  SDL_Renderer *ren;
  
  struct CachedFont {
    SDL::Font handle;
    const char *path;
    int size;
    SDL_Color color;
  };
  
  std::vector<CachedFont> fonts;
};

#endif
