//
//  load_image.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 22/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef utils_load_image_hpp
#define utils_load_image_hpp

#include <memory>
#include <stdexcept>

struct DeleteImageData {
  void operator()(unsigned char *) const noexcept;
};

using ImageData = std::unique_ptr<unsigned char, DeleteImageData>;

struct Image {
  ImageData data;
  int width;
  int height;
};

class ImageError final : std::runtime_error {
public:
  explicit ImageError(const char *) noexcept;
};

Image loadImage(const char *);

#endif
