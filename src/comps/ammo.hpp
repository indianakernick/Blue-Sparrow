//
//  ammo.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_ammo_hpp
#define comps_ammo_hpp

#include <entt/entity/fwd.hpp>

/// The number of missiles that the ship holds
struct MissileAmmo {
  int n;
};

/// The remaining durability of the hull of a ship
struct Hull {
  int h;
};

/// The damage that a missile or blaster bolt deals to its target
struct Damage {
  int d;
  entt::entity killer;
};

/// The number of coins that the player has
struct Coins {
  int c;
};

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
