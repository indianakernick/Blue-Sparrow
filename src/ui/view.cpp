//
//  view.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "view.hpp"

#include <SDL2/SDL_render.h>
#include "../utils/sdl_check.hpp"

void View::init(SDL_Renderer *ren, FontCache &cache) {
  for (auto &child : children) {
    child->init(ren, cache);
  }
}

bool View::event(const SDL_Event &e) {
  for (auto &child : children) {
    if (child->event(e)) return true;
  }
  return false;
}

void View::render(SDL_Renderer *ren, FontCache &cache) {
  for (auto &child : children) {
    const SDL_Rect viewport = child->viewport();
    SDL_CHECK(SDL_RenderSetViewport(ren, &viewport));
    child->render(ren, cache);
  }
}

View *View::addChild(std::unique_ptr<View> view) {
  View *ret = view.get();
  children.push_back(std::move(view));
  return ret;
}

void View::setLayout(const LayoutDir layout) {
  dir = layout;
}

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

using Kids = std::vector<std::unique_ptr<View>>;

SDL_Point evaluateVertical(const Kids &children) {
  int width = 0;
  int height = 0;
  for (auto &child : children) {
    child->evaluate();
    width = std::max(width, child->minWidth());
    height += child->minHeight();
  }
  return {width, height};
}

SDL_Point evaluateHorizontal(const Kids &children) {
  int width = 0;
  int height = 0;
  for (auto &child : children) {
    child->evaluate();
    width += child->minWidth();
    height = std::max(height, child->minHeight());
  }
  return {width, height};
}

void setViewportVertical(const SDL_Rect viewport, const int minHeight, const Kids &children) {
  int extra = viewport.h - minHeight;
  assert(extra >= 0);
  int y = 0;
  for (auto &child : children) {
    const int w = std::min(viewport.w, child->maxWidth());
    const int x = applyAlignment(child->horiAlign(), w, viewport.w);
    const int useExtra = std::min(extra, child->maxHeight() - child->minHeight());
    extra -= useExtra;
    child->setViewport({x, y, w, child->minHeight() + useExtra});
    y += child->minHeight();
  }
}

void setViewportHorizontal(const SDL_Rect viewport, const int minWidth, const Kids &children) {
  int extra = viewport.w - minWidth;
  int x = 0;
  for (auto &child : children) {
    const int h = std::min(viewport.h, child->maxHeight());
    const int y = applyAlignment(child->vertAlign(), h, viewport.h);
    const int useExtra = std::min(extra, child->maxWidth() - child->minWidth());
    extra -= useExtra;
    child->setViewport({x, y, child->minWidth() + useExtra, h});
    x += child->minWidth();
  }
}

}

void View::evaluate() {
  if (children.empty()) return;
  SDL_Point size = {0, 0};
  switch (dir) {
    case LayoutDir::down:
      size = evaluateVertical(children);
      break;
    case LayoutDir::right:
      size = evaluateHorizontal(children);
      break;
  }
  setFixedWidth(size.x);
  setFixedHeight(size.y);
}

void View::setViewport(const SDL_Rect viewport) {
  rect = viewport;
  switch (dir) {
    case LayoutDir::down:
      setViewportVertical(viewport, minHeight(), children);
      break;
    case LayoutDir::right:
      setViewportHorizontal(viewport, minWidth(), children);
      break;
  }
}

SDL_Rect View::viewport() const {
  return rect;
}

VertAlign View::vertAlign() const {
  return vAlign;
}

void View::setVertAlign(const VertAlign align) {
  vAlign = align;
}

HoriAlign View::horiAlign() const {
  return hAlign;
}

void View::setHoriAlign(const HoriAlign align) {
  hAlign = align;
}
