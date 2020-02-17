//
//  font_cache.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 17/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "font_cache.hpp"

FontCache::FontCache(SDL_Renderer *ren)
  : ren{ren} {}

FC_Font *FontCache::load(const char *path, const int size, const SDL_Color color) {
  for (auto &font : fonts) {
    if (size != font.size) continue;
    if (color.r != font.color.r || color.g != font.color.g || color.b != font.color.b || color.a != font.color.a) continue;
    if (std::strcmp(path, font.path) != 0) continue;
    return font.handle.get();
  }
  SDL::Font handle{FC_CreateFont()};
  FC_Font *ret = handle.get();
  FC_LoadFont(ret, ren, path, size, color, TTF_STYLE_NORMAL);
  fonts.push_back({std::move(handle), path, size, color});
  return ret;
}
