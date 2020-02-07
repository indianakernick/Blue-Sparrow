//
//  read physics.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "read physics.hpp"

#include <box2d/b2_body.h>
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

constexpr float rad2deg = 180.0 / 3.14159;

void readPhysicsTransform(entt::registry &reg) {
  reg.view<Physics, SpriteRect>().each([](auto phys, auto &rect) {
    const b2Vec2 pos = phys.body->GetPosition();
    const float angle = phys.body->GetAngle();
    rect.x = (pos.x - phys.width / 2.0) * 10.0;
    rect.y = (pos.y - phys.height / 2.0) * 10.0;
    rect.width = phys.width * 10.0;
    rect.height = phys.height * 10.0;
    rect.angle = angle * rad2deg;
  });
}
