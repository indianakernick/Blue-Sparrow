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
#include <entt/entity/entity.hpp>
#include <box2d/b2_polygon_shape.h>

Physics makeArena(b2World &world, const float width, const float height) {
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
  bodyDef.userData = toUserData(entt::null);
  b2Body *body = world.CreateBody(&bodyDef);
  
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
  
  return {body, width, height};
}

Physics makeScout(b2World &world, const Team team, const entt::entity e) {
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
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, halfWidth * 2.0f, halfHeight * 2.0f};
}

Physics makeSniper(b2World &world, const Team team, const entt::entity e) {
  const float halfWidth = 1.5f;
  const float halfHeight = 1.0f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.angularDamping = 4.0f;
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
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, halfWidth * 2.0f, halfHeight * 2.0f};
}

Physics makeSmallBullet(b2World &world, const Team team, const entt::entity e) {
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
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, halfWidth * 2.0f, halfHeight * 2.0f};
}

Physics makeSmallMissile(b2World &world, const Team team, const entt::entity e) {
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
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, halfWidth * 2.0f, halfHeight * 2.0f};
}

Physics makeAsteroid(b2World &world, const entt::entity e) {
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
  fixDef.restitution = 1.0f;
  fixDef.filter.categoryBits = asteroid_bit;
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, halfWidth * 2.0f, halfHeight * 2.0f};
}
