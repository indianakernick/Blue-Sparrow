//
//  physics.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "physics.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include "../utils/physics.hpp"
#include "../comps/physics.hpp"
#include <entt/entity/registry.hpp>
#include <box2d/b2_polygon_shape.h>

void setArenaPhysics(
  entt::registry &reg,
  const entt::entity e,
  const float width,
  const float height
) {
  const float pad = 10.0f;
  b2Vec2 arenaVerts[4][4] = {
    // Top
    {{-pad, -pad}, {width + pad, -pad}, {width + pad, 0.0f}, {-pad, 0.0f}},
    // Right
    {{width, 0.0f}, {width + pad, 0.0f}, {width + pad, height}, {width, height}},
    // Bottom
    {{-pad, height}, {width + pad, height}, {width + pad, height + pad}, {-pad, height + pad}},
    // Left
    {{-pad, 0.0f}, {0.0f, 0.0f}, {0.0f, height}, {-pad, height}}
  };

  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.userData = toUserData(e);
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  
  const b2Vec2 offset = {width / 2.0f, height / 2.0f};
  for (int i = 0; i != 4; ++i) {
    b2PolygonShape shape;
    for (int j = 0; j != 4; ++j) {
      arenaVerts[i][j] -= offset;
    }
    shape.Set(arenaVerts[i], 4);
    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.filter.categoryBits = arena_bit;
    body->CreateFixture(&fixDef);
  }
  
  reg.assign<Physics>(e, body, width, height);
}

void setSmallShipPhysics(entt::registry &reg, const entt::entity e, const Team team) {
  const float halfWidth = 1.5f;
  const float halfHeight = 1.0f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.angularDamping = 8.0f;
  bodyDef.linearDamping = 0.1f;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(halfWidth, halfHeight);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 1.0f;
  fixDef.restitution = 0.4f;
  fixDef.filter.categoryBits = shipCat(team);
  fixDef.filter.maskBits = shipMsk(team);
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, halfWidth * 2.0f, halfHeight * 2.0f);
}

void setSmallBoltPhysics(entt::registry &reg, const entt::entity e, const Team team) {
  const float halfWidth = 0.5f;
  const float halfHeight = 0.1f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.bullet = true;
  bodyDef.fixedRotation = true;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(halfWidth, halfHeight);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 1.0f;
  fixDef.filter.categoryBits = bulletCat(team);
  fixDef.filter.maskBits = bulletMsk(team);
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, halfWidth * 2.0f, halfHeight * 2.0f);
}

void setSmallMissilePhysics(entt::registry &reg, const entt::entity e, const Team team) {
  const float halfWidth = 0.5f;
  const float halfHeight = 0.2f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.angularDamping = 10.0f;
  bodyDef.linearDamping = 0.0f;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(halfWidth, halfHeight);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 2.0f;
  fixDef.filter.categoryBits = bulletCat(team);
  fixDef.filter.maskBits = bulletMsk(team);
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, halfWidth * 2.0f, halfHeight * 2.0f);
}

void setAsteroidPhysics(entt::registry &reg, const entt::entity e) {
  const float halfWidth = 8.0f;
  const float halfHeight = 8.0f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(halfWidth, halfHeight);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 2.0f;
  fixDef.restitution = 0.2f;
  fixDef.filter.categoryBits = arena_bit;
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, halfWidth * 2.0f, halfHeight * 2.0f);
}

void setDropPhysics(entt::registry &reg, const entt::entity e) {
  const float halfSize = 0.5f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(halfSize, halfSize);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.isSensor = true;
  fixDef.filter.categoryBits = drop_bit;
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, halfSize * 2.0f, halfSize * 2.0f);
}

void setBeaconPhysics(entt::registry &reg, const entt::entity e) {
  const float halfSize = 4.0f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(halfSize, halfSize);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.restitution = 0.1f;
  fixDef.filter.categoryBits = arena_bit;
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, halfSize * 2.0f, halfSize * 2.0f);
}

void setWallPhysics(
  entt::registry &reg,
  const entt::entity e,
  const float width,
  const float height
) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  bodyDef.userData = toUserData(e);
  
  b2PolygonShape shape;
  shape.SetAsBox(width / 2.0f, height / 2.0f);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.filter.categoryBits = arena_bit;
  
  b2Body *body = reg.ctx<b2World>().CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  reg.assign<Physics>(e, body, width, height);
}
