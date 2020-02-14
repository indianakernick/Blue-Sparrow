//
//  resource_path.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "resource_path.hpp"

#include <string>
#include "sdl_check.hpp"
#include <SDL2/SDL_filesystem.h>

const char *res(const char *name) {
  static std::string path;
  path = SDL_CHECK(SDL_GetBasePath());
  path += name;
  return path.c_str();
}
