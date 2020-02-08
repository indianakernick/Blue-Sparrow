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
#include <box2d/b2_polygon_shape.h>

namespace {

constexpr float width = 128.0f;
constexpr float height = 72.0f;
constexpr float pad = 10.0f;

const b2Vec2 arenaVerts[4][4] = {
  // Top
  {{-pad, -pad}, {width + pad, -pad}, {width + pad, 0.0f}, {-pad, 0.0f}},
  // Right
  {{width, 0.0f}, {width + pad, 0.0f}, {width + pad, height}, {width, height}},
  // Bottom
  {{-pad, height}, {width + pad, height}, {width + pad, height + pad}, {-pad, height + pad}},
  // Left
  {{-pad, 0.0f}, {0.0f, 0.0f}, {0.0f, height}, {-pad, height}}
};

}

void makeArena(b2World &world) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_staticBody;
  b2Body *body = world.CreateBody(&bodyDef);
  
  for (int i = 0; i != 4; ++i) {
    b2PolygonShape shape;
    shape.Set(arenaVerts[i], 4);
    b2FixtureDef fixDef;
    fixDef.shape = &shape;
    fixDef.filter.categoryBits = arena_bit;
    body->CreateFixture(&fixDef);
  }
}

Physics makeSmallShip(b2World &world, const Team team) {
  const float halfWidth = 1.5f;
  const float halfHeight = 1.0f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.angularDamping = 8.0f;
  bodyDef.linearDamping = 0.1f;
  
  b2PolygonShape shape;
  shape.SetAsBox(halfWidth, halfHeight);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 1.0f;
  fixDef.filter.categoryBits = shipCat(team);
  fixDef.filter.maskBits = shipMsk(team);
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, halfWidth * 2.0f, halfHeight * 2.0f};
}

Physics makeSmallBullet(b2World &world, const Team team) {
  const float halfWidth = 0.5f;
  const float halfHeight = 0.1f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.bullet = true;
  bodyDef.fixedRotation = true;
  
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

Physics makeSmallMissile(b2World &world, const Team team) {
  const float halfWidth = 0.5f;
  const float halfHeight = 0.2f;
  
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.bullet = true;
  bodyDef.angularDamping = 10.0f;
  bodyDef.linearDamping = 0.1f;
  
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
