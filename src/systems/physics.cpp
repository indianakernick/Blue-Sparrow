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
#include "../utils/each.hpp"
#include "../comps/physics.hpp"
#include <entt/entity/registry.hpp>

void stepPhysics(entt::registry &reg) {
  reg.ctx<b2World>().Step(1.0f/60.0f, 8, 4);
}

void limitVelocity(entt::registry &reg) {
  entt::each(reg, [](Physics phys, VelocityLimit limit) {
    const b2Vec2 vel = phys.body->GetLinearVelocity();
    const float len = vel.Length();
    if (len > limit.vel) {
      phys.body->SetLinearVelocity((limit.vel / len) * vel);
    }
  });
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

void setTransform(entt::registry &reg, const entt::entity e, const b2Vec2 pos, const float angle) {
  reg.get<Physics>(e).body->SetTransform(pos, angle);
}
