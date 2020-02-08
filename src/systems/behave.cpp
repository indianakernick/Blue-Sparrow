//
//  behave.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "behave.hpp"

#include <box2d/b2_body.h>
#include "../utils/each.hpp"
#include "../comps/input.hpp"
#include "../comps/physics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

void behaveBasic(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Target target, MoveInput &move, BlasterInput &blaster, BasicBehaviour behave) {
    if (target.e == entt::null) {
      blaster.fire = false;
      move.forward = move.left = move.right = false;
      return;
    } else {
      blaster.fire = true;
    }
    
    const b2Vec2 targetPos = reg.get<Physics>(target.e).body->GetPosition();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 toTarget = targetPos - shipPos;
    const float aimAngle = std::atan2(toTarget.y, toTarget.x);
    const float aimDist = toTarget.x * toTarget.x + toTarget.y * toTarget.y;
    const float shipAngle = phys.body->GetAngle();
    float toAim = aimAngle - shipAngle;
    while (toAim < -b2_pi) toAim += 2.0f * b2_pi;
    while (toAim > b2_pi) toAim -= 2.0f * b2_pi;
    
    if (b2Abs(toAim) < 2.0f * b2_pi / 180.0f) {
      move.left = move.right = false;
    } else if (toAim < 0.0f) {
      move.right = false;
      move.left = true;
    } else {
      move.left = false;
      move.right = true;
    }
    
    if (aimDist > behave.dist * behave.dist) {
      move.forward = true;
    } else {
      move.forward = false;
    }
  });
}
