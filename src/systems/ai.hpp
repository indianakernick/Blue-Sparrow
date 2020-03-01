//
//  ai.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 28/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_ai_hpp
#define systems_ai_hpp

#include <entt/entity/fwd.hpp>

void thinkBeaconCapture(entt::registry &);
void thinkSniper(entt::registry &);
void thinkMissile(entt::registry &);

#endif
