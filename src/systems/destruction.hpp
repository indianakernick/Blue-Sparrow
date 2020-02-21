//
//  destruction.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 12/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_destruction_hpp
#define systems_destruction_hpp

#include <entt/entity/fwd.hpp>

/// Drop loot from a ship and eventually call reg.destroy
void destroyShip(entt::registry &, entt::entity);
/// Deal damage to a pair of ships that collided with each other
void collideShipPair(entt::registry &, entt::entity, entt::entity, float);
/// Deal damage to a ship that collided with an asteroid or wall
void collideShip(entt::registry &, entt::entity, float);

#endif
