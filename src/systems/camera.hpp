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

struct Camera;

/// Read the position and angle of Physics bodies and update the SpriteRect
void writeSpriteRect(entt::registry &, const Camera &);

/// Read the position and hull level of ships and update the BarRect
void writeHullBarRect(entt::registry &, const Camera &);

/// Read the position and health of beacons and update the BarRect
void writeBeaconBarRect(entt::registry &, const Camera &);

/// Move the Camera to focus on the object with CameraFocus
void moveCamera(entt::registry &, Camera &);

/// Intialize the camera with the arena size
void initializeCamera(Camera &, float, float);

/// Update the camera with the viewport size (window size)
void updateCameraViewport(Camera &, SDL_Rect);

#endif
