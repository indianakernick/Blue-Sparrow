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

constexpr std::uint16_t ally_ship_bit    = 0b0001;
constexpr std::uint16_t ally_bullet_bit  = 0b0010;
constexpr std::uint16_t enemy_ship_bit   = 0b0100;
constexpr std::uint16_t enemy_bullet_bit = 0b1000;

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
    case Team::ally: return ally_ship_bit | enemy_ship_bit | enemy_bullet_bit;
    case Team::enemy: return enemy_ship_bit | ally_ship_bit | ally_bullet_bit;
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
    case Team::ally: return enemy_ship_bit;
    case Team::enemy: return ally_ship_bit;
  }
}

#endif
