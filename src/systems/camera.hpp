//
//  camera.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_camera_hpp
#define systems_camera_hpp

#include <entt/entity/fwd.hpp>

/// Read the position and angle of Physics bodies and update the SpriteRect
void writeSpriteRect(entt::registry &);

/// Read the position hull level of ships and update the BarRect
void writeBarRect(entt::registry &);

/// Move the Camera to focus on the object width CameraFocus
void moveCamera(entt::registry &);

#endif
