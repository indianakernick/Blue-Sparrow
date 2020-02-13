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
#include <entt/entity/registry.hpp>

namespace {

using CollisionPair = std::pair<entt::entity, entt::entity>;
using Collisions = std::vector<CollisionPair>;

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
    reg.set<Collisions>();
  }

  void BeginContact(b2Contact *contact) override {
    const entt::entity a = fromFixture(contact->GetFixtureA());
    const entt::entity b = fromFixture(contact->GetFixtureB());
    reg.ctx<Collisions>().emplace_back(a, b);
  }
  
  void PreSolve(b2Contact *contact, const b2Manifold *) override {
    const std::uint16_t catA = catFromFixture(contact->GetFixtureA());
    const std::uint16_t catB = catFromFixture(contact->GetFixtureB());
    if (catA == asteroid_bit) {
      if (catB == asteroid_bit || catB == arena_bit) {
        return bounce(contact);
      }
    }
    if (catB == asteroid_bit) {
      if (catA == asteroid_bit || catA == arena_bit) {
        return bounce(contact);
      }
    }
  }

private:
  entt::registry &reg;
};

void destroyBody(entt::entity e, entt::registry &reg) {
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

void stepPhysics(entt::registry &reg, const int fps) {
  reg.ctx<b2World>().Step(1.0f / fps, 8, 3);
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
