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

// Objects
//   Walls
//     Could build a simple labrinth
//   Beacons
//     TF2 Control points
//       Team members occupy the area for some amount of time to capture
//       More team members means faster capturing
//     Diep.io Dominators
//       Deal damage to capture
//       The beacon acts like a turret and shoots back
//   Turrets
//     Should beacons shoot?

// AIs
//   (for the different roles of a team)
//   Dodging (missiles that dodge)
//   Picking up drops
//   Defending
//   Guarding
//   Attacking

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
