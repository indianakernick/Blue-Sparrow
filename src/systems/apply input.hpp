//
//  apply input.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef apply_input_hpp
#define apply_input_hpp

#include <entt/entity/fwd.hpp>

/// Read the MoveInput and MoveParams to apply forces to the Physics body
void applyMoveInput(entt::registry &);

#endif
