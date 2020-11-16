//
//  behave.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "behave.hpp"

#include <queue>
#include <cmath>
#include <box2d/b2_body.h>
#include <SDL2/SDL_timer.h>
#include <box2d/b2_world.h>
#include "../utils/each.hpp"
#include "../comps/arena.hpp"
#include <box2d/b2_fixture.h>
#include "../comps/teams.hpp"
#include "../comps/input.hpp"
#include "../comps/params.hpp"
#include "../comps/physics.hpp"
#include "../utils/physics.hpp"
#include "../comps/graphics.hpp"
#include "../comps/behaviour.hpp"
#include <entt/entity/registry.hpp>

namespace {

float normalizeAngle(float angle) {
  while (angle < -b2_pi) angle += 2.0f * b2_pi;
  while (angle > b2_pi) angle -= 2.0f * b2_pi;
  return angle;
}

void cwByAngle(MotionCommand &motion, const float deltaAngle, const float threshold = 0.5f) {
  if (b2Abs(deltaAngle) < threshold * deg2rad) {
    motion.ccw = motion.cw = false;
  } else if (deltaAngle < 0.0f) {
    motion.ccw = true;
    motion.cw = false;
  } else {
    motion.cw = true;
    motion.ccw = false;
  }
}

void forwardByAccel(MotionCommand &motion, const float accel, const float threshold = 0.5f) {
  if (b2Abs(accel) < threshold) {
    motion.forward = motion.reverse = false;
  } else if (accel < 0.0f) {
    motion.reverse = true;
    motion.forward = false;
  } else {
    motion.forward = true;
    motion.reverse = false;
  }
}

void rightByAccel(MotionCommand &motion, const float accel, const float threshold = 0.5f) {
  if (b2Abs(accel) < threshold) {
    motion.left = motion.right = false;
  } else if (accel < 0.0f) {
    motion.left = true;
    motion.right = false;
  } else {
    motion.right = true;
    motion.left = false;
  }
}

b2Vec2 scaleToLength(const b2Vec2 vec, const float length) {
  return (length / vec.Length()) * vec;
}

b2Vec2 normalized(b2Vec2 vec) {
  vec.Normalize();
  return vec;
}

b2Vec2 interseptPoint(
  const b2Vec2 targetPos,
  const b2Vec2 targetVel,
  const b2Vec2 launchPos,
  const float bulletSpeed
) {
  // https://gamedev.stackexchange.com/a/25292/72999
  // TODO: Check the determinent
  // This causes a divide by zero if the bullet can't catch up to the target
  // https://wiki.unity3d.com/index.php/Calculating_Lead_For_Projectiles?_ga=2.202661201.1952245439.1581310561-814066108.1581310561
  
  const b2Vec2 toTarget = targetPos - launchPos;

  const float a = b2Dot(targetVel, targetVel) - bulletSpeed * bulletSpeed;
  const float b = 2.0f * b2Dot(targetVel, toTarget);
  const float c = b2Dot(toTarget, toTarget);

  const float p = -b / (2.0f * a);
  const float q = std::sqrt((b * b) - 4.0f * a * c) / (2.0f * a);

  const float t1 = p - q;
  const float t2 = p + q;
  const float t = (t1 > t2 && t2 > 0.0f ? t2 : t1);

  return targetPos + t * targetVel;
}

}

// TODO: I wonder if I could use the genetic algorithm to train the ultimate bot!

void behaveOrbit(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Target target, MotionCommand &motion, OrbitBehaviour behave) {
    motion.left = motion.right = false;
    
    if (target.e == entt::null) {
      motion.forward = motion.reverse = false;
      motion.ccw = motion.cw = false;
      return;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    
    const b2Vec2 toTarget = normalized(targetPos - shipPos);
    const b2Vec2 relativeVel = targetVel - shipVel;
    // TODO: Is interceptPoint applicable here?
    const b2Vec2 desiredPos = targetPos - behave.dist * toTarget + 0.5f * relativeVel;
    const b2Vec2 desiredVel = scaleToLength(desiredPos - shipPos, behave.speed);
    const b2Vec2 accel = desiredVel - shipVel;
    forwardByAccel(motion, b2Dot(accel, toTarget));
  });
}

void behaveAim(entt::registry &reg) {
  entt::each(reg, [&](
    Physics phys, Target target, MotionCommand &motion,
    BlasterCommand &blaster, BlasterParams params, AimBehaviour behave
  ) {
    if (target.e == entt::null) {
      blaster.fire = false;
      motion.ccw = motion.cw = false;
      return;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    
    b2Vec2 aimPos;
    switch (behave.level) {
      case AimLevel::aim_pos:
        aimPos = targetPos;
        break;
      case AimLevel::aim_ahead:
        aimPos = interseptPoint(targetPos, targetVel - shipVel, shipPos, params.speed);
        break;
    }
    
    const b2Vec2 toAim = aimPos - shipPos;
    const float aimAngle = std::atan2(toAim.y, toAim.x);
    const float angleChange = normalizeAngle(aimAngle - phys.body->GetAngle());
    cwByAngle(motion, angleChange, behave.rotateThreshold);
    blaster.fire = b2Abs(angleChange) < behave.fireThreshold;
  });
}

void behaveSeek(entt::registry &reg) {
  entt::each(reg, [&](Physics phys, Target target, MotionCommand &motion, SeekBehaviour behave) {
    motion.left = motion.right = false;
    
    if (target.e == entt::null) {
      motion.forward = motion.reverse = false;
      motion.ccw = motion.cw = false;
      return;
    }
    
    if (behave.level == SeekLevel::no_aim) {
      motion.forward = true;
      motion.reverse = motion.ccw = motion.cw = false;
      return;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    
    b2Vec2 accel;
    switch (behave.level) {
      case SeekLevel::aim_pos:
        accel = targetPos - shipPos;
        break;
      case SeekLevel::aim_vel_pos: {
        const b2Vec2 desiredVel = scaleToLength(targetPos - shipPos, behave.speed);
        accel = desiredVel - shipVel;
        break;
      }
      case SeekLevel::aim_vel_ahead: {
        const b2Vec2 aimPos = interseptPoint(targetPos, targetVel, shipPos, behave.speed);
        const b2Vec2 desiredVel = scaleToLength(aimPos - shipPos, behave.speed);
        accel = desiredVel - shipVel;
        break;
      }
      case SeekLevel::no_aim:
        assert(false);
    }
    
    const float aimAngle = std::atan2(accel.y, accel.x);
    const float angleChange = normalizeAngle(aimAngle - phys.body->GetAngle());
    cwByAngle(motion, angleChange);
    
    motion.forward = b2Abs(angleChange) < behave.fireThreshold;
    motion.reverse = false;
  });
}

void behaveStationary(entt::registry &reg) {
  entt::each(reg, [](Physics phys, MotionCommand &motion, StationaryBehaviour behave) {
    const b2Vec2 desiredVel = behave.pos - phys.body->GetPosition();
    const b2Vec2 accel = desiredVel - phys.body->GetLinearVelocity();
    const b2Vec2 forwardDir = angleMag(phys.body->GetAngle(), 1.0f);
    const b2Vec2 rightDir = forwardDir.Skew();
    
    forwardByAccel(motion, b2Dot(accel, forwardDir), behave.moveThreshold);
    rightByAccel(motion, b2Dot(accel, rightDir), behave.moveThreshold);
  });
}

namespace {

class QueryCallback final : public b2QueryCallback {
public:
  QueryCallback(entt::registry &reg, const b2Vec2 point)
    : reg{reg}, point{point} {}

  bool ReportFixture(b2Fixture *fix) override {
    entt::entity e = fromUserData(fix->GetBody()->GetUserData());
    if (!reg.valid(e)) return true;
    if (!reg.has<Type>(e)) return true;
    if (reg.get<Type>(e) != Type::ship) return true;
    if (!reg.has<Team>(e)) return true;
    if (reg.get<Team>(e) != Team::enemy) return true;
    if (!fix->TestPoint(point)) return true;
    fixture = fix;
    return false;
  }
  
  b2Fixture *getFixture() const {
    return fixture;
  }
  
private:
  entt::registry &reg;
  b2Vec2 point;
  b2Fixture *fixture = nullptr;
};

}

void behaveMouse(entt::registry &reg) {
  entt::each(reg, [&](entt::entity e, Physics phys, MotionCommand &motion, MouseInput mouse, BlasterParams params) {
    // TODO: Can we avoid using the Camera context variable?
    const auto &cam = reg.ctx<Camera>();
    const b2Vec2 shipPos = phys.body->GetPosition();
    float aimX = mouse.x / cam.zoom + cam.x;
    float aimY = mouse.y / cam.zoom + cam.y;
    
    if (reg.has<AimAssist>(e)) {
      QueryCallback callback{reg, {aimX, aimY}};
      b2AABB aabb;
      aabb.lowerBound = {aimX - 0.001f, aimY - 0.001f};
      aabb.upperBound = {aimX + 0.001f, aimY + 0.001f};
      phys.body->GetWorld()->QueryAABB(&callback, aabb);
      if (b2Fixture *fix = callback.getFixture()) {
        b2Body *body = fix->GetBody();
        const b2Vec2 aim = interseptPoint(
          body->GetPosition(),
          body->GetLinearVelocity() - phys.body->GetLinearVelocity(),
          phys.body->GetPosition(),
          params.speed
        );
        aimX = aim.x;
        aimY = aim.y;
      }
    }
    
    const float aimAngle = std::atan2(
      aimY - shipPos.y,
      aimX - shipPos.x
    );
    cwByAngle(motion, normalizeAngle(aimAngle - phys.body->GetAngle()));
  });
}

#define DEBUG_NAVIGATE 0

#if DEBUG_NAVIGATE

#include <chrono>
#include <iostream>

#endif

namespace {

float angleBetween(const b2Vec2 a, const b2Vec2 b) {
  return std::acos(b2Dot(normalized(a), normalized(b)));
}

/// Implementation of A* search algorithm.
class PathFinder {
public:
  explicit PathFinder(const MapWalls &map)
    : map{map} {
    steps.resize(map.width * map.height);
  }

  void find(const b2Vec2 from, const b2Vec2 to) {
    visited.clear();
    unvisited.clear();
    unvisited.push_back({to, to, 0.0f, 0.0f});
    std::fill(steps.begin(), steps.end(), std::numeric_limits<float>::infinity());
    getSteps(to) = 0.0f;
    
    while (!unvisited.empty()) {
      const Node top = unvisited.front();
      visited.push_back(top);
      if (top.pos == from) return;
      std::pop_heap(unvisited.begin(), unvisited.end());
      unvisited.pop_back();
      
      for (const b2Vec2 dir : neighbors) {
        const b2Vec2 neighborPos = top.pos + dir;
        if (neighborPos == top.prev) continue;
        if (!isWalkable(neighborPos, dir)) continue;
        
        const float neighborSteps = top.steps + calcSteps(dir);
        const float neighborDist = calcDistance(neighborPos, from);
        const float neighborTurn = calcTurn(top.pos - top.prev, neighborPos - top.pos);
        const float neighborCost = neighborSteps + neighborDist + neighborTurn;
        
        float &oldSteps = getSteps(neighborPos);
        if (neighborSteps < oldSteps) {
          if (oldSteps != std::numeric_limits<float>::infinity()) {
            for (auto n = unvisited.begin(); n != unvisited.end(); ++n) {
              if (n->pos == neighborPos) {
                n->prev = top.pos;
                n->steps = neighborSteps;
                n->cost = neighborCost;
                std::push_heap(unvisited.begin(), std::next(n));
                goto Found;
              }
            }
          }
          
          unvisited.push_back({neighborPos, top.pos, neighborSteps, neighborCost});
          std::push_heap(unvisited.begin(), unvisited.end());
          Found:
          oldSteps = neighborSteps;
        }
      }
    }
    
    // No path found
    assert(false);
  }

  template <typename Func>
  void eachPosition(Func func) const {
    #if DEBUG_NAVIGATE
    std::cout << "Path length: " << visited.back().steps << '\n';
    #endif
    
    b2Vec2 prev = visited.back().prev;
    for (auto n = visited.crbegin(); n != visited.crend(); ++n) {
      if (n->pos == prev) {
        func(n->pos);
        prev = n->prev;
      }
    }
  }

private:
  struct Node {
    b2Vec2 pos;
    b2Vec2 prev;
    float steps;
    float cost;
    
    bool operator<(const Node &other) const {
      return cost > other.cost;
    }
  };

  const MapWalls &map;
  std::vector<Node> visited;
  std::vector<Node> unvisited;
  std::vector<float> steps;
  
  static inline const b2Vec2 neighbors[8] = {
    {0.0f, -1.0f},
    {1.0f, -1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {-1.0f, 1.0f},
    {-1.0f, 0.0f},
    {-1.0f, -1.0f}
  };
  
  static float calcSteps(const b2Vec2 dir) {
    return dir.Length();
  }
  
  static float calcDistance(const b2Vec2 a, const b2Vec2 b) {
    return b2Distance(a, b);
  }
  
  static float calcTurn(const b2Vec2 prevDir, const b2Vec2 nextDir) {
    return angleBetween(prevDir, nextDir) * 0.1f;
  }
  
  bool isWall(const b2Vec2 pos) const {
    if (pos.x < 0 || pos.y < 0) return true;
    if (pos.x >= map.width || pos.y >= map.height) return true;
    return map.data[pos.y * map.width + pos.x];
  }
  
  bool isWalkable(const b2Vec2 to, const b2Vec2 dir) const {
    if (b2Abs(dir) == b2Vec2{1.0f, 1.0f}) {
      if (isWall({to.x - dir.x, to.y}) || isWall({to.x, to.y - dir.y})) return false;
    }
    return !isWall(to);
  }
  
  float &getSteps(const b2Vec2 pos) {
    return steps[pos.y * map.width + pos.x];
  }
};

}

#if DEBUG_NAVIGATE

namespace {

using Clock = std::chrono::system_clock;
Clock::time_point start, end;
bool done = false;

float getSeconds(Clock::duration duration) {
  const long millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return std::round(millis / 100.0f) / 10.f;
}

}

#endif

void behaveNavigate(entt::registry &reg) {
  const MapWalls &map = reg.ctx<MapWalls>();
  PathFinder &finder = reg.ctx_or_set<PathFinder>(map);
  const float scale = map.scale;
  const float invScale = 1.0f / scale;
  const b2Vec2 center = {map.width / 2.0f, map.height / 2.0f};
  
  auto tilePos = [=](b2Vec2 pos) {
    pos = center + invScale * pos;
    return b2Vec2{std::floor(pos.x), std::floor(pos.y)};
  };
  auto worldPos = [=](const b2Vec2 pos) {
    return scale * (pos - center + b2Vec2{0.5f, 0.5f});
  };
  auto floorPos = [=](b2Vec2 pos) {
    pos *= scale;
    pos = b2Vec2{std::floor(pos.x), std::floor(pos.y)};
    return invScale * pos;
  };
  
  const std::uint32_t now = SDL_GetTicks();
  entt::each(reg, [&](Physics phys, MotionCommand &motion, MotionParams params, NavigateBehaviour &behave) {
    const b2Vec2 shipPos = phys.body->GetPosition();
    
    if (SDL_TICKS_PASSED(now, behave.timeout)) {
      behave.path.clear();
      behave.timeout = now + 1500;
      
      #if DEBUG_NAVIGATE
      std::cout << "Retry\n";
      #endif
    }
    
    if (behave.path.empty()) {
      const b2Vec2 fromPos = tilePos(shipPos);
      const b2Vec2 toPos = tilePos(behave.target);
      if (fromPos == toPos) {
        behave.path.push_back(floorPos(behave.target));
      } else {
        #if DEBUG_NAVIGATE
        Clock::time_point astarStart = Clock::now();
        #endif
        
        finder.find(fromPos, toPos);
        
        #if DEBUG_NAVIGATE
        Clock::time_point astarEnd = Clock::now();
        std::cout << "A* " << std::chrono::duration_cast<std::chrono::microseconds>(astarEnd - astarStart).count() << "us\n";
        #endif
        
        finder.eachPosition([&](const b2Vec2 pos) {
          behave.path.push_back(worldPos(pos));
        });
      }
      
      #if DEBUG_NAVIGATE
      start = Clock::now();
      #endif
    }
    
    if (behave.path.size() > 1) {
      if (b2DistanceSquared(shipPos, behave.path[0]) < 2.0f * 2.0f) {
        behave.path.erase(behave.path.begin());
        behave.timeout = now + 1500;
      } else if (b2DistanceSquared(shipPos, behave.path[1]) < 6.0f * 6.0f) {
        behave.path.erase(behave.path.begin());
        behave.timeout = now + 1500;
      }
    }
    
    b2Vec2 desiredVel = 2.0f * (behave.path[0] - shipPos);
    if (behave.path.size() > 1) {
      desiredVel = behave.path[0] + behave.path[1] - 2.0f * shipPos;
      const float angle = angleBetween(behave.path[1] - behave.path[0], behave.path[0] - shipPos);
      if (angle < b2_pi / 3.0f) {
        desiredVel = scaleToLength(desiredVel, 1.05f * params.speed);
      }
    }
    
    #if DEBUG_NAVIGATE
    else {
      if (!std::exchange(done, true)) {
        end = Clock::now();
        std::cout << "Travel time: " << getSeconds(end - start) << " seconds\n";
      }
    }
    #endif
    
    #if DEBUG_NAVIGATE
    if (behave.debug0 != entt::null) {
      reg.get<Physics>(behave.debug0).body->SetTransform(behave.path[0], 0.0f);
    }
    if (behave.debug1 != entt::null) {
      const b2Vec2 point = behave.path.size() > 1 ? behave.path[1] : behave.path[0];
      reg.get<Physics>(behave.debug1).body->SetTransform(point, 0.0f);
    }
    #endif
    
    const b2Vec2 accel = desiredVel - phys.body->GetLinearVelocity();
    const b2Vec2 forwardDir = angleMag(phys.body->GetAngle(), 1.0f);
    const b2Vec2 rightDir = forwardDir.Skew();
    
    forwardByAccel(motion, b2Dot(accel, forwardDir));
    rightByAccel(motion, b2Dot(accel, rightDir));
    
    if (behave.look) {
      const float aimAngle = std::atan2(desiredVel.y, desiredVel.x);
      cwByAngle(motion, normalizeAngle(aimAngle - phys.body->GetAngle()));
    }
  });
}

void behaveIdle(entt::registry &reg) {
  entt::each(reg, [&](MotionCommand &motion, IdleBehaviour) {
    motion.forward = motion.reverse = false;
    motion.ccw = motion.cw = false;
    motion.left = motion.right = false;
  });
}

void behavePacifist(entt::registry &reg) {
  entt::each(reg, [&](BlasterCommand &blaster, PacifistBehaviour) {
    blaster.fire = false;
  });
}
