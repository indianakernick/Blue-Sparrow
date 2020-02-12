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

void destroyShip(entt::registry &, entt::entity);
void collideShipPair(entt::registry &, entt::entity, entt::entity);
void collideShip(entt::registry &, entt::entity, entt::entity);

#endif
