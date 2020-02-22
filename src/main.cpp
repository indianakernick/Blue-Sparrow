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
//   Turrets
//     Should beacons shoot?

// Camera
//   All ships should use view distance for targeting
//   View distance might be larger for snipers

// UI
//   Might need a minimap
//     Should include the status of the beacons

// Game mechanics
//   Winning and losing
//   Respawning
//   Healing over time

// AIs
//   (for the different roles of a team)
//   Dodging (missiles that dodge)
//   Picking up drops
//   Defending
//   Guarding
//   Attacking
//   AIs should only target the player if they can actually see the player
//     shooting the player from way across the map is really unfair

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
