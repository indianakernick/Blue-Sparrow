//
//  layout_item.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 15/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef ui_layout_item_hpp
#define ui_layout_item_hpp

class LayoutItem {
public:
  int minWidth() const;
  int minHeight() const;
  
  int maxWidth() const;
  int maxHeight() const;

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
};

#endif
