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

void View::setLayout(const Layout newLayout) {
  layout = newLayout;
}

void View::setSize(const Size newSize) {
  setWidth(newSize.width);
  setHeight(newSize.height);
}

void View::setWidth(const AxisSize width) {
  assert(width.grow >= 0);
  assert(width.shrink >= 0);
  size.width = width;
}

void View::setHeight(const AxisSize height) {
  assert(height.grow >= 0);
  assert(height.shrink >= 0);
  size.height = height;
}

SDL_Rect View::viewport() const {
  return rect;
}

void View::setViewport(const SDL_Rect viewport) {
  rect = viewport;
  switch (layout.dir) {
    case LayoutDir::right:
      return setViewport<LayoutDir::right>();
    case LayoutDir::down:
      return setViewport<LayoutDir::down>();
  }
}

namespace {

template <LayoutDir>
struct AxisTraits;

template <>
struct AxisTraits<LayoutDir::right> {
  using CrossTraits = AxisTraits<LayoutDir::down>;
  static constexpr auto mainDir = LayoutDir::right;
  
  static AxisSize mainSize(const Size &size) {
    return size.width;
  }
  static AxisSize crossSize(const Size &size) {
    return size.height;
  }
  
  static int mainRectSize(const SDL_Rect &rect) {
    return rect.w;
  }
  static int crossRectSize(const SDL_Rect &rect) {
    return rect.h;
  }
  
  static SDL_Rect viewport(
    const SDL_Rect &rect,
    const int mainOffset,
    const int crossOffset,
    const int mainSize,
    const int crossSize
  ) {
    return {rect.x + mainOffset, rect.y + crossOffset, mainSize, crossSize};
  }
};

template <>
struct AxisTraits<LayoutDir::down> {
  using CrossTraits = AxisTraits<LayoutDir::right>;
  static constexpr auto mainDir = LayoutDir::down;
  
  static AxisSize mainSize(const Size &size) {
    return size.height;
  }
  static AxisSize crossSize(const Size &size) {
    return size.width;
  }
  
  static int mainRectSize(const SDL_Rect &rect) {
    return rect.h;
  }
  static int crossRectSize(const SDL_Rect &rect) {
    return rect.w;
  }
  
  static SDL_Rect viewport(
    const SDL_Rect &rect,
    const int mainOffset,
    const int crossOffset,
    const int mainSize,
    const int crossSize
  ) {
    return {rect.x + crossOffset, rect.y + mainOffset, crossSize, mainSize};
  }
};

}

template <LayoutDir Dir>
void View::setViewport() const {
  using Traits = AxisTraits<Dir>;

  const int mainSize = Traits::mainRectSize(rect);
  const int crossSize = Traits::crossRectSize(rect);
  ChildStats stats = getChildStats<Traits>();
  
  applyMainFactors<Traits>(mainSize - stats.totalSpace, stats);
  
  const AlignOffset mainAlign = applyMainAlign(mainSize - stats.totalSpace);
  int offset = mainAlign.start;
  
  for (size_t i = 0; i != children.size(); ++i) {
    auto &child = *children[i];
    const int crossSpace = child.applyCrossFactor<Traits>(crossSize);
    const int crossExtra = crossSize - crossSpace;
    
    child.setViewport(Traits::viewport(
      rect, offset, applyCrossAlign(crossExtra), stats.space[i], crossSpace
    ));
    
    offset += stats.space[i];
    offset += mainAlign.between;
  }
}

template <typename Traits>
int View::baseSize() const {
  if (Traits::mainSize(size).basis < 0) {
    if (Traits::mainDir == layout.dir) {
      int sum = 0;
      for (auto &child : children) {
        sum += child->baseSize<Traits>();
      }
      return sum;
    } else {
      int max = 0;
      for (auto &child : children) {
        max = std::max(max, child->baseSize<Traits>());
      }
      return max;
    }
  } else {
    return Traits::mainSize(size).basis;
  }
}

template <typename Traits>
View::ChildStats View::getChildStats() const {
  ChildStats stats = {
    0, 0, 0, std::vector<int>(children.size(), 0)
  };
  for (size_t i = 0; i != children.size(); ++i) {
    View &child = *children[i];
    const int space = child.baseSize<Traits>();
    stats.totalGrow += Traits::mainSize(child.size).grow;
    stats.totalShrink += Traits::mainSize(child.size).shrink;
    stats.totalSpace += space;
    stats.space[i] = space;
  }
  return stats;
}

View::AlignOffset View::applyMainAlign(const int extra) const {
  const int childCount = static_cast<int>(children.size());
  switch (layout.main) {
    case MainAlign::start:
      return {0, 0};
    case MainAlign::end:
      return {extra, 0};
    case MainAlign::center:
      return {extra / 2, 0};
    case MainAlign::space_between:
      return {0, extra / (childCount - 1)};
    case MainAlign::space_around:
      return {extra / (childCount * 2), extra / childCount};
    case MainAlign::space_evenly:
      return {extra / (childCount + 1), extra / (childCount + 1)};
  }
}

int View::applyCrossAlign(const int extra) const {
  switch (layout.cross) {
    case CrossAlign::start:
      return 0;
    case CrossAlign::end:
      return extra;
    case CrossAlign::center:
      return extra / 2;
  }
}

template <typename Traits, int (AxisSize::*Factor)>
void View::applyMainFactor(const int extra, ChildStats &stats, const int total) const {
  for (size_t i = 0; i != children.size(); ++i) {
    const int factor = Traits::mainSize(children[i]->size).*Factor;
    const int used = (extra * factor) / total;
    stats.totalSpace += used;
    stats.space[i] += used;
  }
}

template <typename Traits>
void View::applyMainFactors(const int extra, ChildStats &stats) const {
  if (extra > 0 && stats.totalGrow > 0) {
    applyMainFactor<Traits, &AxisSize::grow>(extra, stats, stats.totalGrow);
  } else if (extra < 0 && stats.totalShrink > 0) {
    applyMainFactor<Traits, &AxisSize::shrink>(extra, stats, stats.totalShrink);
  }
}

template <typename Traits>
int View::applyCrossFactor(const int crossSize) const {
  int crossSpace = baseSize<typename Traits::CrossTraits>();
  int crossExtra = crossSize - crossSpace;
                        
  if (crossExtra > 0 && Traits::crossSize(size).grow > 0) {
    crossSpace = crossSize;
  } else if (crossExtra < 0 && Traits::crossSize(size).shrink > 0) {
    crossSpace = crossSize;
  }
  
  return crossSpace;
}
