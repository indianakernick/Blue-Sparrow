//
//  window.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_window_hpp
#define ui_window_hpp

#include "view.hpp"

struct SDL_Window;

class Window : public View {
public:
  explicit Window(SDL_Window *);

  bool event(const SDL_Event &e) override;

  void setInitialViewport();
  
private:
  SDL_Window *window;
};

#endif
