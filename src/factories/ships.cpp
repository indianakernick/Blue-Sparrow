//
//  ships.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "ships.hpp"

#include "physics.hpp"
#include "upgrades.hpp"
#include "../comps/ai.hpp"
#include "../comps/ammo.hpp"
#include "../comps/drops.hpp"
#include "../comps/input.hpp"
#include "../comps/timers.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

entt::entity makePlayer(entt::registry &reg) {
  entt::entity e = reg.create();
  setUpgradableHull(reg, e, 0);
  setShip(reg, e, Team::ally);
  setSmallShipPhysics(reg, e, Team::ally);
  setPlayer(reg, e);
  setScoutSprite(reg, e, Team::ally);
  setUpgradableMotion(reg, e, 0);
  setUpgradableBlaster(reg, e, 0);
  setUpgradableMissile(reg, e, 0);
  reg.assign<ViewDistance>(e, 30.0f);
  return e;
}

entt::entity makeScout(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  if (team == Team::enemy) {
    reg.assign<Drops>(e, 4, 5, 0, 1, 3, 5);
  }
  setHull(reg, e, 0);
  setShip(reg, e, team);
  setSmallShipPhysics(reg, e, team);
  setBeaconCaptureAI(reg, e);
  setScoutSprite(reg, e, team);
  setMotion(reg, e, 0);
  setBlaster(reg, e, 0);
  reg.assign<ViewDistance>(e, 30.0f);
  return e;
}

entt::entity makeSniper(entt::registry &reg, const Team team) {
  entt::entity e = reg.create();
  if (team == Team::enemy) {
    reg.assign<Drops>(e, 10, 14, 0, 2, 4, 6);
  }
  setHull(reg, e, 0);
  setShip(reg, e, team);
  setSmallShipPhysics(reg, e, team);
  setSniperSprite(reg, e, team);
  setSniperBehave(reg, e);
  setSniperMotion(reg, e);
  setSniperBlaster(reg, e);
  reg.assign<ViewDistance>(e, 50.0f);
  return e;
}

void setShip(entt::registry &reg, const entt::entity e, const Team team) {
  reg.assign<Team>(e, team);
  reg.assign<Type>(e, Type::ship);
}

void setPlayer(entt::registry &reg, const entt::entity e) {
  reg.assign<CameraFocus>(e);
  reg.assign<KeyInput>(e);
  reg.assign<MouseInput>(e);
  reg.assign<AimAssist>(e);
  reg.assign<Coins>(e, 0);
}

void setBeaconCaptureAI(entt::registry &reg, const entt::entity e) {
  reg.assign<BeaconCaptureAI>(e);
  reg.assign<Target>(e);
}

void setSniperBehave(entt::registry &reg, const entt::entity e) {
  reg.assign<SniperBehaviour>(e, 0.0f, 0.0f);
  reg.assign<Target>(e);
  reg.assign<TargetEnemyShip>(e, true);
}

namespace {

Sprite teamSprite(const Team team) {
  switch (team) {
    case Team::ally: return {63, 63, 191};
    case Team::enemy: return {63, 191, 63};
  }
}

}

void setScoutSprite(entt::registry &reg, const entt::entity e, const Team team) {
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, teamSprite(team));
}

void setSniperSprite(entt::registry &reg, const entt::entity e, const Team team) {
  reg.assign<SpriteRect>(e);
  reg.assign<Sprite>(e, teamSprite(team));
}

void setSniperMotion(entt::registry &reg, const entt::entity e) {
  MotionParams motionParams;
  motionParams.forwardForce = 150.0f;
  motionParams.reverseForce = 150.0f;
  motionParams.lateralForce = 150.0f;
  motionParams.turnTorque = 100.0f;
  motionParams.speed = 15.0f;
  reg.assign<MotionParams>(e, motionParams);
  reg.assign<MotionCommand>(e);
  reg.assign<VelocityLimit>(e, 15.0f);
}

void setSniperBlaster(entt::registry &reg, const entt::entity e) {
  BlasterParams blasterParams;
  blasterParams.rof = 0.25f;
  blasterParams.speed = 160.0f;
  blasterParams.damage = 30;
  blasterParams.spread = 0.0f;
  reg.assign<BlasterParams>(e, blasterParams);
  reg.assign<BlasterCommand>(e);
  reg.assign<BlasterTimer>(e, std::uint32_t{});
}

void setSniperPosition(
  entt::registry &reg,
  const entt::entity e,
  const float x,
  const float y
) {
  auto &behave = reg.get<SniperBehaviour>(e);
  behave.x = x;
  behave.y = y;
}
