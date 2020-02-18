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

union SDL_Event;
struct SDL_Renderer;
class FontCache;

// The view layout system is inspired by CSS Flexbox
// https://css-tricks.com/snippets/css/a-guide-to-flexbox/

/// The direction that child views are layed out (flex-direction).
/// Defines the main axis (perpendicular to the cross axis)
enum class LayoutDir : std::uint8_t {
  /// Main axis starts on the left and ends on the right
  right,
  /// Main axis starts at the top and ends at the bottom
  down
};

/// The alignment the child views on the main axis (justify-content)
enum class MainAlign : std::uint8_t {
  /// Children are packed toward the start
  start,
  /// Children are packed toward the end
  end,
  /// Children are packed in the center
  center,
  /// Children are spread out in a line
  space_between,
  /// Children are spread out in a line with equal space around them
  space_around,
  /// Children are spread out in a line with equal space between each other
  space_evenly
};

/// The alignment of the child views on the cross axis (align-items)
enum class CrossAlign : std::uint8_t {
  /// Children are packed toward the start
  start,
  /// Children are packed toward the end
  end,
  /// Children are packed in the center
  center
};

/// Parameters for defining the relative position of child views
struct Layout {
  LayoutDir dir = LayoutDir::right;
  MainAlign main = MainAlign::start;
  CrossAlign cross = CrossAlign::start;
};

/// The parameters for defining size on a single axis
struct AxisSize {
  /// The base size of the view (flex-basis). A negative value results in the
  /// base size being the sum of the base sizes of the children
  int basis = -1;
  /// The factor by which this view will grow relative to siblings (flex-grow)
  int grow = 0;
  /// The factor by which this view will shrink relative to siblings (flex-shrink)
  int shrink = 1;
};

// TODO: child override CrossAlign (align-self)?
// should child views define or override CrossAlign?

/// Parameters for defining the size of a view
struct Size {
  AxisSize width;
  AxisSize height;
};

class View {
public:
  virtual ~View() = default;
  
  virtual void init(SDL_Renderer *, FontCache &);
  virtual bool event(const SDL_Event &);
  virtual void update(float);
  virtual void render(SDL_Renderer *, FontCache &);
  
  View *addChild(std::unique_ptr<View>);
  
  template <typename V>
  std::enable_if_t<!std::is_same_v<V, View>, V *>
  addChild(std::unique_ptr<V> view) {
    V *ret = view.get();
    addChild(std::unique_ptr<View>{std::move(view)});
    return ret;
  }
  
  void setLayout(Layout);
  void setSize(Size);
  
  void setWidth(AxisSize);
  void setHeight(AxisSize);
  
  SDL_Rect viewport() const;
  void setViewport(SDL_Rect);

private:
  std::vector<std::unique_ptr<View>> children;
  SDL_Rect rect;
  Layout layout;
  Size size;
  
  template <LayoutDir>
  void setViewport() const;
  
  template <typename>
  int baseSize() const;
  
  struct ChildStats {
    int totalGrow;
    int totalShrink;
    int totalSpace;
    std::vector<int> space;
  };
  
  template <typename>
  ChildStats getChildStats() const;
  
  struct AlignOffset {
    int start;
    int between;
  };
  
  AlignOffset applyMainAlign(int) const;
  int applyCrossAlign(int) const;
  
  template <typename, int (AxisSize::*)>
  void applyMainFactor(int, ChildStats &, int) const;
  
  template <typename>
  void applyMainFactors(int, ChildStats &) const;
  
  template <typename>
  int applyCrossFactor(int) const;
};

#endif
