//
//  layout_item.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_layout_item_hpp
#define ui_layout_item_hpp

#include <SDL2/SDL_rect.h>

class LayoutItem {
public:
  // I don't think we need this
  virtual ~LayoutItem() = default;
  
  virtual void evaluate() {}
  virtual void setViewport(SDL_Rect);

  int minWidth() const;
  int minHeight() const;
  
  int maxWidth() const;
  int maxHeight() const;
  
  SDL_Rect viewport() const;

protected:
  void setFixedWidth(int);
  void setFixedHeight(int);
  
  void setGrowWidth(int);
  void setGrowHeight(int);
  
  void setWidth(int, int);
  void setHeight(int, int);

private:
  int minW = 1;
  int minH = 1;
  int maxW = 1;
  int maxH = 1;
  SDL_Rect view;
};

#endif
