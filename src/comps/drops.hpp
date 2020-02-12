//
//  drops.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 12/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_drops_hpp
#define comps_drops_hpp

/// Coin drop
struct Coin {};

/// Missile ammo drop
struct Ammo {};

/// Hull repair drop
struct Scrap {};

/// Definition of what a ship drops.
/// The number of items dropped is a random number within a range.
struct Drops {
  int minCoins;
  int maxCoins;
  int minAmmo;
  int maxAmmo;
  int minScrap;
  int maxScrap;
};

#endif
