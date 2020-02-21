//
//  collisions.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 21/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_collisions_hpp
#define comps_collisions_hpp

#include <vector>
#include <entt/entity/fwd.hpp>

struct CollisionPair {
  entt::entity a;
  entt::entity b;
};

using CollisionPairs = std::vector<CollisionPair>;

struct PostCollisionPair {
  entt::entity a;
  entt::entity b;
  float impulse;
};

using PostCollisionPairs = std::vector<PostCollisionPair>;

#endif
