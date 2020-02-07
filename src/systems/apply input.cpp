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
#include "../factories/bolt.hpp"
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

void applyBlasterInput(entt::registry &reg) {
  reg.view<Physics, BlasterParams, BlasterInput>().each([&](Physics phys, BlasterParams params, BlasterInput &input) {
    if (!input.fire) return;
    input.fire = false;
    
    const b2Vec2 shipPos = phys.body->GetPosition();
    const float shipAngle = phys.body->GetAngle();
    const b2Vec2 shipDir = angleMag(shipAngle, 1.0f);
    const b2Vec2 boltPos = shipPos + phys.width * shipDir;
    
    entt::entity bolt = makeBolt(reg);
    b2Body *boltBody = reg.get<Physics>(bolt).body;
    boltBody->SetTransform(boltPos, shipAngle);
    boltBody->SetLinearVelocity(params.speed * shipDir);
  });
}
