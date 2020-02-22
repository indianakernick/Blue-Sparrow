//
//  graphics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_graphics_hpp
#define comps_graphics_hpp

struct SDL_Renderer;
struct SDL_Texture;

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

/// The position of a bar on the screen (used for hull bars)
struct BarRect {
  int x, y;
  int width, height;
  float progress;
};

struct BackgroundSprite {};

/// Indicates which object the camera should focus on
struct CameraFocus {};

/// The maximum number of meters that a ship (not only the player) can see in
/// either axis
struct ViewDistance {
  float max;
};

/// A context component describing the camera
struct Camera {
  float x;           // offset meters
  float y;           // offset meters
  int width;         // viewport pixels
  int height;        // viewport pixels
  float arenaWidth;  // arena meters
  float arenaHeight; // arena meters
  float zoom;        // pixels per meter
  float minZoom;     // pixels per meter
};

/// A context component describing the rendering context
struct Drawing {
  SDL_Renderer *ren;
  SDL_Texture *fgTex;
  SDL_Texture *bgTex;
};

#endif
