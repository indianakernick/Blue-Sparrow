//
//  ai.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 27/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef comps_ai_hpp
#define comps_ai_hpp

// I think AIs should be able to communicate.
// I don't want every instance of the same AI to try to do the same thing.
// I'm not sure yet if this will be an issue and it sounds difficult to implement
// so I'll leave it for now.

/*

behaviour for aiming
behaviour for moving
beahviour for shooting

pursue behaviour
  this is like navigate except that we aim at the target instead of forward
  can we create this behaviour by composing the existing ones?

capture beacons
  if there are enemy ships nearby,
    shoot them
  otherwise,
    shoot the beacon
  pursue enemies near the beacon but don't get too far from the beacon
  when the beacon is captured,
    either defend it or try to capture another

defend beacon
  stay near the beacon
  shoot nearby enemies
  don't get too far from the beacon

shoot enemies
  visit each of the beacons looking for enemies along the way
  shoot and pursue any enemy ships spotted
  if the hull gets too damaged,
    return home

*/

struct BeaconCaptureAI {
  enum class State {
    idle,
    navigate,
    shoot_beacon
  } state = State::idle;
};

struct SniperAI {};

struct MissileAI {};

#endif
