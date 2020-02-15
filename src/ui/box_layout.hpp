//
//  box_layout.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_box_layout_hpp
#define ui_box_layout_hpp

#include <vector>
#include "layout_item.hpp"

enum class HoriAlign {
  left, center, right
};

enum class VertAlign {
  top, middle, bottom
};

class VertBoxLayout final : public LayoutItem {
public:
  VertBoxLayout();

  void append(LayoutItem *, HoriAlign = HoriAlign::left);
  void evaluate() override;
  void setViewport(SDL_Rect) override;
  
private:
  struct Item {
    LayoutItem *ptr;
    HoriAlign align;
  };

  std::vector<Item> items;
};

class HoriBoxLayout final : public LayoutItem {
public:
  HoriBoxLayout();
  
  void append(LayoutItem *, VertAlign = VertAlign::top);
  void evaluate() override;
  void setViewport(SDL_Rect) override;

private:
  struct Item {
    LayoutItem *ptr;
    VertAlign align;
  };
  
  std::vector<Item> items;
};

#endif
