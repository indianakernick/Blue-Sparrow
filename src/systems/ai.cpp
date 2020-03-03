//
//  ai.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 28/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "ai.hpp"

#include <box2d/b2_body.h>
#include "find_target.hpp"
#include "../comps/ai.hpp"
#include "../utils/each.hpp"
#include "../comps/arena.hpp"
#include "../comps/teams.hpp"
#include "../comps/physics.hpp"
#include "../comps/behaviour.hpp"

namespace {

bool suitableBeacon(const BeaconState state, const Team team) {
  switch (state) {
    case BeaconState::ally:
      return team == Team::enemy;
    case BeaconState::enemy:
      return team == Team::ally;
    case BeaconState::neutral:
      return true;
  }
}

bool suitableTargetBeacon(entt::registry &reg, const entt::entity e) {
  const entt::entity beacon = reg.get<BeaconCaptureAI>(e).beacon;
  const BeaconState state = reg.get<Beacon>(beacon).state;
  const Team team = reg.get<Team>(e);
  return suitableBeacon(state, team);
}

class BeaconOrder {
public:
  BeaconOrder(const Team team)
    : team{team}, foundOrder{initialOrder(team)} {}

  bool closer(const int order) {
    if (better(order)) {
      foundOrder = order;
      return true;
    } else {
      return false;
    }
  }

private:
  Team team;
  int foundOrder;
  
  static int initialOrder(const Team team) {
    switch (team) {
      case Team::ally:
        return std::numeric_limits<int>::max();
      case Team::enemy:
        return -1;
    }
  }
  
  bool better(const int order) const {
    switch (team) {
      case Team::ally:
        return order < foundOrder;
      case Team::enemy:
        return order > foundOrder;
    }
  }
};

entt::entity findBeacon(entt::registry &reg, const Team team) {
  entt::entity found = entt::null;
  BeaconOrder order{team};
  entt::each(reg, [&](entt::entity e, Beacon beacon) {
    // TODO: Be smarter about choosing beacons
    // Prioritise beacons based on a number of factors
    // like distance, order, damage, neutral vs enemy
    if (suitableBeacon(beacon.state, team)) {
      if (order.closer(beacon.order)) {
        found = e;
      }
    }
  });
  return found;
}

}

// TODO: Make state transitions less tedious
// Remembering which components need to be removed and added is tricky
// Is that just how state machines are?

// TODO: Perhaps there should be a state for navigating with an enemy nearby
// and navigating without and enemy nearby

void thinkBeaconCapture(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, BeaconCaptureAI &ai) {
    switch (ai.state) {
      case BeaconCaptureAI::State::init:
        ai.state = BeaconCaptureAI::State::idle;
        reg.assign<IdleBehaviour>(e);
        reg.assign<PacifistBehaviour>(e);
        break;
    
      case BeaconCaptureAI::State::idle: {
        const entt::entity beacon = findBeacon(reg, reg.get<Team>(e));
        if (beacon != entt::null) {
          ai.state = BeaconCaptureAI::State::navigate;
          const b2Vec2 pos = reg.get<Physics>(beacon).body->GetPosition();
          reg.assign<NavigateBehaviour>(e, pos);
          reg.remove<IdleBehaviour>(e);
          ai.beacon = beacon;
        }
        break;
      }
      
      case BeaconCaptureAI::State::navigate:
        if (reg.get<NavigateBehaviour>(e).path.size() == 1) {
          ai.state = BeaconCaptureAI::State::shoot_beacon;
          const float speed = reg.get<VelocityLimit>(e).vel;
          reg.assign<OrbitBehaviour>(e, 10.0f, speed);
          reg.assign_or_replace<AimBehaviour>(e, AimBehaviour{AimLevel::aim_pos});
          reg.remove_if_exists<PacifistBehaviour>(e);
          reg.remove<NavigateBehaviour>(e);
          reg.get<Target>(e).e = ai.beacon;
        } else if (!suitableTargetBeacon(reg, e)) {
          ai.state = BeaconCaptureAI::State::idle;
          reg.remove<NavigateBehaviour>(e);
          reg.remove_if_exists<AimBehaviour>(e);
          reg.assign_or_replace<PacifistBehaviour>(e);
          reg.assign<IdleBehaviour>(e);
          reg.get<Target>(e).e = entt::null;
        } else {
          entt::entity &target = reg.get<Target>(e).e;
          target = findNearestEnemyShip(reg, e, true);
          if (target == entt::null) {
            reg.remove_if_exists<AimBehaviour>(e);
            reg.assign_or_replace<PacifistBehaviour>(e);
            reg.get<NavigateBehaviour>(e).look = true;
          } else {
            // TODO: Update this when updating EnTT
            // https://github.com/skypjack/entt/commit/17d96427ea4e7120c98f2eb6b36a5ed9c9e1e650
            reg.remove_if_exists<PacifistBehaviour>(e);
            reg.assign_or_replace<AimBehaviour>(e, AimBehaviour{AimLevel::aim_ahead});
            reg.get<NavigateBehaviour>(e).look = false;
          }
        }
        break;
        
      case BeaconCaptureAI::State::shoot_beacon:
        // should do findNearestEnemyShip(reg, e, false);
        if (!suitableTargetBeacon(reg, e)) {
          ai.state = BeaconCaptureAI::State::idle;
          reg.assign<IdleBehaviour>(e);
          reg.remove<OrbitBehaviour>(e);
          reg.remove_if_exists<AimBehaviour>(e);
          reg.assign<PacifistBehaviour>(e);
        }
        break;
    }
  });
}

// TODO: Limit the rate that the target changes
// Must keep a target for a minimum amount of time before changing
// when there are lots of targets nearby, the ship can change between them too
// fast to aim properly

void thinkSniper(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, Target &target, SniperAI) {
    target.e = findNearestEnemyShip(reg, e, true);
  });
}

void thinkMissile(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, Target &target, MissileAI) {
    if (target.e == entt::null) {
      target.e = findNearestEnemyShip(reg, e, true);
    }
  });
}
