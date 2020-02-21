//
//  physics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 10/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef utils_physics_hpp
#define utils_physics_hpp

#include <box2d/b2_math.h>
#include <entt/entity/fwd.hpp>

constexpr float rad2deg = 180.0f / b2_pi;
constexpr float deg2rad = b2_pi / 180.0f;

inline b2Vec2 angleMag(const float angle, const float mag) {
  return {cosf(angle) * mag, sinf(angle) * mag};
}

static_assert(sizeof(void *) >= sizeof(entt::entity));

inline void *toUserData(const entt::entity entity) {
  void *userData{};
  std::memcpy(&userData, &entity, sizeof(entt::entity));
  return userData;
}

inline entt::entity fromUserData(void *userData) {
  entt::entity entity{};
  std::memcpy(&entity, &userData, sizeof(entt::entity));
  return entity;
}

#endif
