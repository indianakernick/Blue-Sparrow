//
//  ai.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 28/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "ai.hpp"

#include <box2d/b2_body.h>
#include "../comps/ai.hpp"
#include "../utils/each.hpp"
#include "../comps/arena.hpp"
#include "../comps/teams.hpp"
#include "../comps/physics.hpp"
#include "../comps/behaviour.hpp"

namespace {

// TODO: Similar logic in find_target

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

void thinkBeaconCapture(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, BeaconCaptureAI &ai) {
    switch (ai.state) {
      case BeaconCaptureAI::State::idle: {
        entt::entity beacon = findBeacon(reg, reg.get<Team>(e));
        if (beacon != entt::null) {
          ai.state = BeaconCaptureAI::State::navigate;
          const b2Vec2 pos = reg.get<Physics>(beacon).body->GetPosition();
          reg.assign<NavigateBehaviour>(e, pos.x, pos.y);
        }
        break;
      }
      case BeaconCaptureAI::State::navigate:
        if (reg.get<NavigateBehaviour>(e).path.size() == 1) {
          ai.state = BeaconCaptureAI::State::shoot_beacon;
          const float speed = reg.get<VelocityLimit>(e).vel;
          reg.assign<OrbitBehaviour>(e, 10.0f, speed, OrbitLevel::aim_pos);
          reg.assign<TargetBeacon>(e);
          reg.remove<NavigateBehaviour>(e);
        }
        // TODO: Stop navigating if the target is nolonger a suitable beacon
        break;
      case BeaconCaptureAI::State::shoot_beacon:
        if (reg.get<Target>(e).e == entt::null) {
          ai.state = BeaconCaptureAI::State::idle;
          reg.remove<TargetBeacon>(e);
          reg.remove<OrbitBehaviour>(e);
        }
        break;
    }
  });
}
