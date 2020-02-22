//
//  load_image.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 22/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "load_image.hpp"

#include "stb_image.h"

void DeleteImageData::operator()(unsigned char *ptr) const noexcept {
  stbi_image_free(ptr);
}

ImageError::ImageError(const char *error) noexcept
  : std::runtime_error{error} {}

namespace {

[[noreturn]] void raise() {
  const char *error = stbi_failure_reason();
  throw ImageError{error};
}

}

Image loadImage(const char *path) {
  int width, height;
  ImageData data{stbi_load(path, &width, &height, nullptr, 4)};
  if (data == nullptr) raise();
  return {std::move(data), width, height};
}
