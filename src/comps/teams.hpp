//
//  teams.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_teams_hpp
#define comps_teams_hpp

#include <cstdint>

constexpr std::uint16_t ally_ship_bit    = 1 << 0;
constexpr std::uint16_t ally_bullet_bit  = 1 << 1;
constexpr std::uint16_t enemy_ship_bit   = 1 << 2;
constexpr std::uint16_t enemy_bullet_bit = 1 << 3;
constexpr std::uint16_t arena_bit        = 1 << 4;
constexpr std::uint16_t asteroid_bit     = 1 << 5;

enum class Team {
  enemy,
  ally
};

enum class Type {
  ship,
  bullet
};

constexpr std::uint16_t shipCat(const Team team) {
  switch (team) {
    case Team::ally: return ally_ship_bit;
    case Team::enemy: return enemy_ship_bit;
  }
}

constexpr std::uint16_t shipMsk(const Team team) {
  switch (team) {
    case Team::ally: return ~ally_bullet_bit;
    case Team::enemy: return ~enemy_bullet_bit;
  }
}

constexpr std::uint16_t bulletCat(const Team team) {
  switch (team) {
    case Team::ally: return ally_bullet_bit;
    case Team::enemy: return enemy_bullet_bit;
  }
}

constexpr std::uint16_t bulletMsk(const Team team) {
  switch (team) {
    case Team::ally: return enemy_ship_bit | asteroid_bit;
    case Team::enemy: return asteroid_bit;//ally_ship_bit | asteroid_bit;
  }
}

#endif
