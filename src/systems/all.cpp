//
//  all.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 13/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "all.hpp"

#include "camera.hpp"
#include "render.hpp"
#include "expire.hpp"
#include "behave.hpp"
#include "physics.hpp"
#include "collisions.hpp"
#include "find_target.hpp"
#include "apply_commands.hpp"

void prePhysicsSystems(entt::registry &reg) {
  findTarget(reg);
  behaveOrbit(reg);
  behaveSeek(reg);
  behaveSniper(reg);
  behaveMouse(reg);
  applyMoveCommands(reg);
  applyBlasterCommands(reg);
  applyMissileCommands(reg);
  expireTemporary(reg);
}

void postPhysicsSystems(entt::registry &reg) {
  limitVelocity(reg);
  handleCollisions(reg);
  moveCamera(reg);
  writeSpriteRect(reg);
  writeBarRect(reg);
}

void renderSystems(entt::registry &reg) {
  renderBackground(reg);
  renderSprite(reg);
  renderBar(reg);
}
