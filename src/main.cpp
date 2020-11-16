//
//  main.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "ui/application.hpp"

int main() {
  Application app;
  app.run();
}

// Code
//   Should we abandon entt::each?
//   Move constants into one place
//     1500 milliseconds is one example
//     there are a lot of them
//   Profiling

// Objects
//   Turrets
//     Should beacons shoot?

// UI
//   Might need a minimap
//     Should include the status of the beacons
//   Should the health bar of a beacon indicate ally/enemy/neutral?

// Game mechanics
//   Winning and losing
//   Respawning
//   Healing over time
//   Maybe the ability to grab onto asteroids and move them around

// AIs
//   (for the different roles of a team)
//   Avoiding walls (missiles should do this)
//   Dodging (missiles should do this)
//   Picking up drops
//   Defending
//   Guarding
//   Attacking
//   High level AI
//     We have behaviours (seek and orbit) that define an immediate goal
//     We have levels that define how well the immediate goal is reached
//     We need some way of switching between behaviours dynamically
//     Example:
//       if there are no nearby targets, wander around
//       if a target is spotted, engage in the target
//   Can/should we separate aiming behaviours from moving behaviours?

// Drops
//   AIs that seek out drops
//   Drops for each team maybe?
//   Drops only for the player maybe?
//   Currently, drops are accidently picked up by other ships
//   Perhaps drops could spawn in a few places
//     Similar to TF2

// Controls
//   Should we move away from digital controls and use analog controls?
//     Would make AIs a little bit more effective
//     AIs can take full advantage of it but players only get improved aiming
//     Might help improve aim assist for the player
//     https://gamedev.stackexchange.com/questions/1083/what-makes-aiming-in-a-console-first-person-shooter-feel-good/1325#1325

// Weapons
//   Drop mines that only effect the enemy
//     Perhaps fancy mines could have tiny blasters on them
//   Pulsing laser
//     Bouncing the laser off of mirrors could provide some interesting gameplay

// CMake
//   https://cmake.org/cmake/help/v3.11/module/FetchContent.html
//   https://discourse.libsdl.org/t/deploying-for-macos-how-are-we-supposed-to-do-it/23897
//   https://github.com/urho3d/Urho3D/tree/master/CMake/Toolchains
