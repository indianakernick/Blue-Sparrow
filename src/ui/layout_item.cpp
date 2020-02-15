//
//  layout_item.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "layout_item.hpp"

#include <limits>
#include <cassert>

int LayoutItem::minWidth() const {
  return minW;
}

int LayoutItem::minHeight() const {
  return minH;
}

int LayoutItem::maxWidth() const {
  return maxW;
}

int LayoutItem::maxHeight() const {
  return maxH;
}

SDL_Rect LayoutItem::viewport() const {
  return view;
}

void LayoutItem::setViewport(const SDL_Rect viewport) {
  assert(viewport.x >= 0);
  assert(viewport.y >= 0);
  assert(viewport.w > 0);
  assert(viewport.h > 0);
  view = viewport;
}

void LayoutItem::setFixedWidth(const int w) {
  assert(w > 0);
  minW = maxW = w;
}

void LayoutItem::setFixedHeight(const int h) {
  assert(h > 0);
  minH = maxH = h;
}

void LayoutItem::setGrowWidth(const int w) {
  assert(w > 0);
  minW = w;
  maxW = std::numeric_limits<int>::max();
}

void LayoutItem::setGrowHeight(const int h) {
  assert(h > 0);
  minH = h;
  maxH = std::numeric_limits<int>::max();
}

void LayoutItem::setWidth(const int min, const int max) {
  assert(min > 0);
  assert(max > 0);
  minW = min;
  maxW = max;
}

void LayoutItem::setHeight(const int min, const int max) {
  assert(min > 0);
  assert(max > 0);
  minH = min;
  maxH = max;
}
