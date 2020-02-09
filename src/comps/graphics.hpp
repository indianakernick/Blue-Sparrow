//
//  graphics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_graphics_hpp
#define comps_graphics_hpp

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

/// Indicates which object the camera should focus on
struct CameraFocus {};

struct Camera {
  float centerX;
  float centerY;
  int width;
  int height;
  int arenaWidth;
  int arenaHeight;
};

#endif
