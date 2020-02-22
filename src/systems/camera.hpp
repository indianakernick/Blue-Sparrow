//
//  camera.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef systems_camera_hpp
#define systems_camera_hpp

#include <SDL2/SDL_rect.h>
#include <entt/entity/fwd.hpp>

/// Read the position and angle of Physics bodies and update the SpriteRect
void writeSpriteRect(entt::registry &);

/// Read the position and hull level of ships and update the BarRect
void writeHullBarRect(entt::registry &);

/// Read the position and health of beacons and update the BarREct
void writeBeaconBarRect(entt::registry &);

/// Move the Camera to focus on the object width CameraFocus
void moveCamera(entt::registry &);

void initializeCamera(entt::registry &, float, float);
void updateCameraViewport(entt::registry &, SDL_Rect);
void updateCameraBackground(entt::registry &, SDL_Point);

#endif
