//
//  layout_engine.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 14/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_layout_engine_hpp
#define ui_layout_engine_hpp

#include <SDL2/SDL_rect.h>

union SDL_Event;
struct SDL_Window;

class LayoutEngine {
public:
  void init(SDL_Window *);
  bool event(const SDL_Event &);

  SDL_Rect gameViewport() const;
  SDL_Rect statsViewport() const;

private:
  SDL_Rect viewport;
};

#endif
