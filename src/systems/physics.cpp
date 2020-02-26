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
#include "../comps/teams.hpp"
#include <box2d/b2_contact.h>
#include "../utils/physics.hpp"
#include "../comps/physics.hpp"
#include "../comps/collisions.hpp"
#include <entt/entity/registry.hpp>

namespace {

entt::entity fromFixture(b2Fixture *fixture) {
  return fromUserData(fixture->GetBody()->GetUserData());
}

std::uint16_t catFromFixture(b2Fixture *fixture) {
  return fixture->GetFilterData().categoryBits;
}

void bounce(b2Contact *contact) {
  contact->SetRestitution(1.0f);
  contact->SetFriction(0.0f);
}

class ContactListener final : public b2ContactListener {
public:
  explicit ContactListener(entt::registry &reg)
    : reg{reg} {
    reg.set<CollisionPairs>();
    reg.set<PostCollisionPairs>();
  }

  void BeginContact(b2Contact *contact) override {
    const entt::entity a = fromFixture(contact->GetFixtureA());
    const entt::entity b = fromFixture(contact->GetFixtureB());
    reg.ctx<CollisionPairs>().push_back({a, b});
  }
  
  void PreSolve(b2Contact *contact, const b2Manifold *) override {
    const std::uint16_t catA = catFromFixture(contact->GetFixtureA());
    const std::uint16_t catB = catFromFixture(contact->GetFixtureB());
    
    if (catA == arena_bit && catB == arena_bit) {
      bounce(contact);
    }
  }
  
  void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override {
    const entt::entity a = fromFixture(contact->GetFixtureA());
    const entt::entity b = fromFixture(contact->GetFixtureB());
    float sum = 0.0f;
    for (int32 i = 0; i != impulse->count; ++i) {
      sum += impulse->normalImpulses[i];
    }
    reg.ctx<PostCollisionPairs>().push_back({a, b, sum});
  }

private:
  entt::registry &reg;
};

void destroyBody(entt::registry &reg, entt::entity e) {
  b2Body *body = reg.get<Physics>(e).body;
  if (body) body->GetWorld()->DestroyBody(body);
}

}

void initializePhysics(entt::registry &reg) {
  static ContactListener listener{reg};
  b2World &world = reg.set<b2World>(b2Vec2{0.0f, 0.0f});
  world.SetContactListener(&listener);
  reg.on_destroy<Physics>().connect<&destroyBody>();
}

void stepPhysics(entt::registry &reg, const float delta) {
  reg.ctx<b2World>().Step(delta, 8, 3);
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

void setTransform(entt::registry &reg, const entt::entity e, const b2Vec2 pos, const float angle) {
  reg.get<Physics>(e).body->SetTransform(pos, angle);
}

void setMotion(entt::registry &reg, const entt::entity e, const b2Vec2 vel, const float rot) {
  b2Body *body = reg.get<Physics>(e).body;
  body->SetLinearVelocity(vel);
  body->SetAngularVelocity(rot);
}
