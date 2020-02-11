//
//  player.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef factories_player_hpp
#define factories_player_hpp

#include <entt/entity/fwd.hpp>

entt::entity makePlayer(entt::registry &);

// Single file for all ship factories
// functions like makeSmallShip, setPlayer, setOrbiter

// have sets of parameters in levels
// like level 1 blasters
// level 1 movement
// upgrade system will just choose levels

// classes of ships
// like scouts and snipers

#endif
