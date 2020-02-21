//
//  physics.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_physics_hpp
#define comps_physics_hpp

class b2Body;

/// A Box2D physics body
struct Physics {
  b2Body *body = nullptr;
  float width, height;
};

/// The maximum speed that the physics body will be allowed to move
struct VelocityLimit {
  float vel;
};

#endif
