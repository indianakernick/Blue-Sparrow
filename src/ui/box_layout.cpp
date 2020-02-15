//
//  box_layout.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "box_layout.hpp"

namespace {

int applyAlignment(const HoriAlign align, const int inner, const int outer) {
  switch (align) {
    case HoriAlign::left:   return 0;
    case HoriAlign::center: return (outer - inner) / 2;
    case HoriAlign::right:  return outer - inner;
  }
}

int applyAlignment(const VertAlign align, const int inner, const int outer) {
  return applyAlignment(static_cast<HoriAlign>(align), inner, outer);
}

}

VertBoxLayout::VertBoxLayout() {
  setFixedWidth(1);
  setFixedHeight(1);
}

void VertBoxLayout::append(LayoutItem *item, const HoriAlign align) {
  assert(item);
  items.push_back({item, align});
}

void VertBoxLayout::evaluate() {
  int width = 0;
  int height = 0;
  for (Item item : items) {
    item.ptr->evaluate();
    width = std::max(width, item.ptr->minWidth());
    height += item.ptr->minHeight();
  }
  setFixedWidth(width);
  setFixedHeight(height);
}

void VertBoxLayout::setViewport(const SDL_Rect viewport) {
  LayoutItem::setViewport(viewport);
  int extra = viewport.h - minHeight();
  int y = 0;
  for (Item item : items) {
    const int w = std::min(viewport.w, item.ptr->maxWidth());
    const int x = applyAlignment(item.align, w, viewport.w);
    const int useExtra = std::min(extra, item.ptr->maxHeight() - item.ptr->minHeight());
    extra -= useExtra;
    item.ptr->setViewport({x, y, w, item.ptr->minHeight() + useExtra});
    y += item.ptr->minHeight();
  }
}

HoriBoxLayout::HoriBoxLayout() {
  setFixedWidth(1);
  setFixedHeight(1);
}

void HoriBoxLayout::append(LayoutItem *item, const VertAlign align) {
  assert(item);
  items.push_back({item, align});
}

void HoriBoxLayout::evaluate() {
  int width = 0;
  int height = 0;
  for (Item item : items) {
    item.ptr->evaluate();
    width += item.ptr->minWidth();
    height = std::max(height, item.ptr->minHeight());
  }
  // TODO: We might need a way to choose between setFixedWidth and the others
  setFixedWidth(width);
  setFixedHeight(height);
}

void HoriBoxLayout::setViewport(const SDL_Rect viewport) {
  LayoutItem::setViewport(viewport);
  int extra = viewport.w - minWidth();
  int x = 0;
  for (Item item : items) {
    const int h = std::min(viewport.h, item.ptr->maxHeight());
    const int y = applyAlignment(item.align, h, viewport.h);
    const int useExtra = std::min(extra, item.ptr->maxWidth() - item.ptr->minWidth());
    extra -= useExtra;
    item.ptr->setViewport({x, y, item.ptr->minWidth() + useExtra, h});
    x += item.ptr->minWidth();
  }
}
