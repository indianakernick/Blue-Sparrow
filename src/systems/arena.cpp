//
//  arena.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 21/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "arena.hpp"

#include "../comps/arena.hpp"
#include "../comps/teams.hpp"

void damageBeacon(Beacon &beacon, const Team shooter, const int damage) {
  switch (beacon.state) {
    case BeaconState::ally:
      if (shooter == Team::ally) return;
      beacon.ally -= damage;
      if (beacon.ally <= 0) {
        beacon.state = BeaconState::enemy;
        beacon.ally = 0;
        beacon.enemy = beacon.max;
      }
      break;
    case BeaconState::enemy:
      if (shooter == Team::enemy) return;
      beacon.enemy -= damage;
      if (beacon.enemy <= 0) {
        beacon.state = BeaconState::ally;
        beacon.enemy = 0;
        beacon.ally = beacon.max;
      }
      break;
    case BeaconState::neutral:
      beacon.neutral -= damage;
      switch (shooter) {
        case Team::ally:
          beacon.ally += damage;
          break;
        case Team::enemy:
          beacon.enemy += damage;
          break;
      }
      if (beacon.neutral <= 0) {
        if (beacon.ally > beacon.enemy) {
          beacon.ally = beacon.max;
          beacon.enemy = 0;
          beacon.neutral = 0;
          beacon.state = BeaconState::ally;
        } else if (beacon.enemy > beacon.ally) {
          beacon.ally = 0;
          beacon.enemy = beacon.max;
          beacon.neutral = 0;
          beacon.state = BeaconState::enemy;
        } else {
          // Next hit wins
        }
      }
      break;
  }
}
