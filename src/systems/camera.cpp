//
//  camera.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "camera.hpp"

#include <box2d/b2_body.h>
#include "../utils/each.hpp"
#include "../comps/ammo.hpp"
#include "../comps/arena.hpp"
#include "../comps/params.hpp"
#include "../utils/physics.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include <entt/entity/registry.hpp>

void writeSpriteRect(entt::registry &reg) {
  auto cam = reg.ctx<Camera>();
  entt::each(reg, [=](Physics phys, SpriteRect &rect) {
    const b2Vec2 pos = phys.body->GetPosition();
    const float angle = phys.body->GetAngle();
    rect.x = (pos.x - phys.width / 2.0 - cam.x) * cam.zoom;
    rect.y = (pos.y - phys.height / 2.0 - cam.y) * cam.zoom;
    rect.width = phys.width * cam.zoom;
    rect.height = phys.height * cam.zoom;
    rect.angle = angle * rad2deg;
  });
}

void writeHullBarRect(entt::registry &reg) {
  auto cam = reg.ctx<Camera>();
  entt::each(reg, [&](Physics phys, Hull hull, HullParams params, BarRect &rect) {
    const float yOffset = (phys.width + phys.height) / 4.0f;
    const b2Vec2 pos = phys.body->GetPosition();
    rect.width = 50;
    rect.height = 4;
    rect.x = (pos.x           - cam.x) * cam.zoom - rect.width / 2 + 0.5f;
    rect.y = (pos.y - yOffset - cam.y) * cam.zoom                  + 0.5f;
    rect.progress = static_cast<float>(hull.h) / params.durability;
  });
}

void writeBeaconBarRect(entt::registry &reg) {
  auto cam = reg.ctx<Camera>();
  entt::each(reg, [&](Physics phys, Beacon beacon, BarRect &rect, Sprite &sprite) {
    const float yOffset = phys.height * 2.0f / 3.0f;
    const b2Vec2 pos = phys.body->GetPosition();
    rect.width = 150;
    rect.height = 4;
    rect.x = (pos.x           - cam.x) * cam.zoom - rect.width / 2 + 0.5f;
    rect.y = (pos.y - yOffset - cam.y) * cam.zoom                  + 0.5f;
    switch (beacon.state) {
      // TODO: Should probably set beacon sprite in separate system
      // TODO: Should also have separate system for rendering ships (MoveCommand)
      case BeaconState::ally:
        sprite = Sprite{0, 0, 255};
        rect.progress = static_cast<float>(beacon.ally) / beacon.max;
        break;
      case BeaconState::enemy:
        sprite = Sprite{255, 0, 0};
        rect.progress = static_cast<float>(beacon.enemy) / beacon.max;
        break;
      case BeaconState::neutral:
        sprite = Sprite{255, 255, 0};
        rect.progress = static_cast<float>(beacon.neutral) / beacon.max;
        break;
    }
  });
}

void moveCamera(entt::registry &reg) {
  auto &cam = reg.ctx<Camera>();
  entt::each(reg, [&](Physics phys, CameraFocus) {
    // TODO: Make this less jittery
    const b2Vec2 pos = phys.body->GetPosition();
    const b2Vec2 vel = phys.body->GetLinearVelocity();
    const b2Vec2 futurePos = pos + 0.0f * vel;
    const float width = cam.width / cam.zoom;
    const float height = cam.height / cam.zoom;
    
    cam.x = futurePos.x - width / 2.0f;
    cam.y = futurePos.y - height / 2.0f;
    cam.x = std::max(cam.x, -cam.arenaWidth / 2.0f);
    cam.x = std::min(cam.x, cam.arenaWidth / 2.0f - width);
    cam.y = std::max(cam.y, -cam.arenaHeight / 2.0f);
    cam.y = std::min(cam.y, cam.arenaHeight / 2.0f - height);
  });
}

void initializeCamera(entt::registry &reg, const float arenaSize) {
  Camera cam;
  cam.x = 0.0f;
  cam.y = 0.0f;
  cam.arenaWidth = arenaSize;
  cam.arenaHeight = arenaSize;
  cam.zoom = INFINITY;
  reg.set<Camera>(cam);
}

void updateCameraViewport(entt::registry &reg, const SDL_Rect viewport) {
  auto &cam = reg.ctx<Camera>();
  cam.width = viewport.w;
  cam.height = viewport.h;
  cam.minZoom = std::max(viewport.w / cam.arenaWidth, viewport.h / cam.arenaHeight);
  cam.zoom = std::max(cam.zoom, cam.minZoom);
}

void updateCameraBackground(entt::registry &reg, const SDL_Point bgSize) {
  auto &cam = reg.ctx<Camera>();
  cam.maxZoom = std::min(bgSize.x / cam.arenaWidth, bgSize.y / cam.arenaHeight);
  cam.zoom = std::min(cam.zoom, cam.maxZoom);
}
