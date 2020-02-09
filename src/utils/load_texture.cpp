//
//  load_texture.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 9/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "load_texture.hpp"

#include <memory>
#include "stb_image.h"
#include "sdl_check.hpp"
#include <SDL2/SDL_render.h>

namespace {

struct DeleteImage {
  void operator()(stbi_uc *ptr) const noexcept {
    stbi_image_free(ptr);
  }
};

using Image = std::unique_ptr<stbi_uc, DeleteImage>;

class ImageError final : std::runtime_error {
public:
  explicit ImageError(const char *error) noexcept
    : std::runtime_error{error} {}
};

[[noreturn]] void raise() {
  const char *error = stbi_failure_reason();
  throw ImageError{error};
}

}

SDL::Texture loadTexture(SDL_Renderer *renderer, const char *path) {
  int width, height;
  Image data{stbi_load(path, &width, &height, nullptr, 4)};
  if (data == nullptr) {
    raise();
  }
  SDL::Texture tex{SDL_CHECK(SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ABGR8888,
    SDL_TEXTUREACCESS_STATIC,
    width, height
  ))};
  SDL_CHECK(SDL_UpdateTexture(tex.get(), nullptr, data.get(), width * 4));
  return tex;
}
