//
//  bolt.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "bolt.hpp"

#include "../comps/timers.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"

#include <box2d/b2_body.h>
#include <box2d/b2_world.h>
#include <box2d/b2_fixture.h>
#include <box2d/b2_polygon_shape.h>

#include <entt/entity/registry.hpp>

#include <SDL2/SDL_timer.h>

namespace {

Physics makePhysics(b2World &world) {
  b2BodyDef bodyDef;
  bodyDef.type = b2_dynamicBody;
  bodyDef.bullet = true;
  bodyDef.fixedRotation = true;
  
  b2PolygonShape shape;
  shape.SetAsBox(0.5f, 0.1f);
  
  b2FixtureDef fixDef;
  fixDef.shape = &shape;
  fixDef.density = 1.0f;
  
  b2Body *body = world.CreateBody(&bodyDef);
  body->CreateFixture(&fixDef);
  return {body, 1.0f, 0.2f};
}

}

entt::entity makeBolt(entt::registry &reg) {
  entt::entity e = reg.create();
  reg.assign<Physics>(e, makePhysics(reg.ctx<b2World>()));
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, Sprite{255, 0, 0});
  reg.assign<ExpireTimer>(e, SDL_GetTicks() + 10000);
  return e;
}
