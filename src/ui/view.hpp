//
//  view.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 16/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_view_hpp
#define ui_view_hpp

#include <memory>
#include <vector>
#include <SDL2/SDL_rect.h>
#include "layout_item.hpp"

union SDL_Event;
struct SDL_Renderer;
class FontCache;

enum class LayoutDir {
  down,
  right
};

enum class VertAlign {
  top, middle, bottom
};

enum class HoriAlign {
  left, center, right
};

class View : public LayoutItem {
public:
  virtual ~View() = default;
  
  virtual void init(SDL_Renderer *, FontCache &);
  virtual bool event(const SDL_Event &);
  virtual void render(SDL_Renderer *, FontCache &);
  
  View *addChild(std::unique_ptr<View>);
  
  template <typename V>
  std::enable_if_t<!std::is_same_v<V, View>, V *>
  addChild(std::unique_ptr<V> view) {
    V *ret = view.get();
    addChild(std::unique_ptr<View>{std::move(view)});
    return ret;
  }
  
  void setLayout(LayoutDir);
  
  void evaluate();
  void setViewport(SDL_Rect);
  SDL_Rect viewport() const;

  VertAlign vertAlign() const;
  void setVertAlign(VertAlign);
  HoriAlign horiAlign() const;
  void setHoriAlign(HoriAlign);

private:
  std::vector<std::unique_ptr<View>> children;
  LayoutDir dir = LayoutDir::down;
  SDL_Rect rect;
  VertAlign vAlign;
  HoriAlign hAlign;
};

#endif
