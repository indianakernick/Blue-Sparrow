//
//  player.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef player_hpp
#define player_hpp

#include <entt/entity/fwd.hpp>

class b2World;

entt::entity makePlayer(entt::registry &, b2World &);

#endif
