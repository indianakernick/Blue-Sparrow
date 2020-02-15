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
class LayoutItem;

class LayoutEngine {
public:
  explicit LayoutEngine(SDL_Window *);
  
  bool event(const SDL_Event &);
  void setRoot(LayoutItem *);
  void evaluate();
  void setInitialViewport();

private:
  SDL_Window *window;
  LayoutItem *root = nullptr;
};

#endif
