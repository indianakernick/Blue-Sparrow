//
//  graphics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef graphics_hpp
#define graphics_hpp

/// The position of a sprite on the screen as a rotated rectangle
struct SpriteRect {
  float x, y;
  float width, height;
  float angle;
};

struct Sprite {
  // In future this will contain the sprite ID
  unsigned char r, g, b;
};

#endif
