//
//  physics.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "physics.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include "../comps/physics.hpp"
#include <entt/entity/registry.hpp>

void stepPhysics(entt::registry &reg) {
  reg.ctx<b2World>().Step(1.0f/60.0f, 8, 4);
}

namespace {

void destroyBody(entt::entity e, entt::registry &reg) {
  b2Body *body = reg.get<Physics>(e).body;
  if (body) body->GetWorld()->DestroyBody(body);
}

}

void connectDestroyBody(entt::registry &reg) {
  reg.on_destroy<Physics>().connect<&destroyBody>();
}
