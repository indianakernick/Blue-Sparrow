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
