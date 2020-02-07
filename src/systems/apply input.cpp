//
//  apply input.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "apply input.hpp"

#include <box2d/b2_body.h>
#include "../comps/input.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include <entt/entity/registry.hpp>

namespace {

b2Vec2 angleMag(const float angle, const float mag) {
  return {std::cos(angle) * mag, std::sin(angle) * mag};
}

}

void applyMoveInput(entt::registry &reg) {
  reg.view<Physics, MoveParams, MoveInput>().each([](auto phys, auto params, auto input) {
    if (input.forward) {
      const b2Vec2 force = angleMag(phys.body->GetAngle(), params.thrustForce);
      phys.body->ApplyForceToCenter(force, true);
    }
    if (input.left && !input.right) {
      phys.body->ApplyTorque(-params.turnTorque, true);
    }
    if (input.right && !input.left) {
      phys.body->ApplyTorque(params.turnTorque, true);
    }
  });
}
