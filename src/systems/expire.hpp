//
//  expire.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef expire_hpp
#define expire_hpp

#include <entt/entity/fwd.hpp>

/// Destroy temporary objects that have expired their lifetimes
void expireTemporary(entt::registry &);

#endif
