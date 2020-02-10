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
#include <box2d/b2_contact.h>
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

namespace {

using CollisionPair = std::pair<entt::entity, entt::entity>;
using Collisions = std::vector<CollisionPair>;

entt::entity fromUserData(void *userData) {
  entt::entity e;
  std::memcpy(&e, &userData, sizeof(entt::entity));
  return e;
}

entt::entity fromUserData(b2Fixture *fixture) {
  return fromUserData(fixture->GetBody()->GetUserData());
}

class ContactListener final : public b2ContactListener {
public:
  explicit ContactListener(entt::registry &reg)
    : reg{reg} {
    reg.set<Collisions>();
  }

  void BeginContact(b2Contact *contact) override {
    const entt::entity a = fromUserData(contact->GetFixtureA());
    const entt::entity b = fromUserData(contact->GetFixtureB());
    reg.ctx<Collisions>().emplace_back(a, b);
  }

private:
  entt::registry &reg;
};

}

void connectContactListener(entt::registry &reg) {
  static ContactListener listener{reg};
  reg.ctx<b2World>().SetContactListener(&listener);
}

void setTransform(entt::registry &reg, const entt::entity e, const b2Vec2 pos, const float angle) {
  reg.get<Physics>(e).body->SetTransform(pos, angle);
}
