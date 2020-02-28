//
//  behave.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 8/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "behave.hpp"

#include <queue>
#include <box2d/b2_body.h>
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
  auto view = reg.view<Physics, Target, MotionCommand, BlasterCommand, OrbitBehaviour, BlasterParams>();
  view.each([&](auto phys, auto target, auto &motion, auto &blaster, auto behave, auto params) {
    motion.left = motion.right = false;
    
    if (target.e == entt::null) {
      blaster.fire = false;
      motion.forward = motion.reverse = false;
      motion.ccw = motion.cw = false;
      return;
    } else {
      blaster.fire = true;
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
    
    b2Vec2 aimPos;
    switch (behave.level) {
      case OrbitLevel::aim_pos:
        aimPos = targetPos;
        break;
      case OrbitLevel::aim_ahead:
        aimPos = interseptPoint(targetPos, targetVel - shipVel, shipPos, params.speed);
        break;
    }
    
    const b2Vec2 toAim = aimPos - shipPos;
    const float aimAngle = std::atan2(toAim.y, toAim.x);
    cwByAngle(motion, normalizeAngle(aimAngle - phys.body->GetAngle()));
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
    
    motion.forward = (b2Abs(angleChange) < b2_pi / 4.0f);
    motion.reverse = false;
  });
}

void behaveSniper(entt::registry &reg) {
  auto view = reg.view<Physics, Target, MotionCommand, BlasterCommand, SniperBehaviour, BlasterParams>();
  view.each([&](auto phys, auto target, auto &motion, auto &blaster, auto behave, auto params) {
    if (target.e == entt::null) {
      blaster.fire = false;
      motion.forward = motion.reverse = false;
      motion.ccw = motion.cw = false;
      motion.left = motion.right = false;
      return;
    } else {
      blaster.fire = true;
    }
    
    b2Body *targetBody = reg.get<Physics>(target.e).body;
    const b2Vec2 targetPos = targetBody->GetPosition();
    const b2Vec2 targetVel = targetBody->GetLinearVelocity();
    const b2Vec2 shipPos = phys.body->GetPosition();
    const b2Vec2 shipVel = phys.body->GetLinearVelocity();
    const float shipAngle = phys.body->GetAngle();
    
    const b2Vec2 desiredVel = b2Vec2{behave.x, behave.y} - shipPos;
    const b2Vec2 accel = desiredVel - shipVel;
    const b2Vec2 forwardDir = angleMag(shipAngle, 1.0f);
    const b2Vec2 rightDir = forwardDir.Skew();
    forwardByAccel(motion, b2Dot(accel, forwardDir), 0.2f);
    rightByAccel(motion, b2Dot(accel, rightDir), 0.2f);
    
    const b2Vec2 aimPos = interseptPoint(targetPos, targetVel - shipVel, shipPos, params.speed);
    const b2Vec2 toAim = aimPos - shipPos;
    const float aimAngle = std::atan2(toAim.y, toAim.x);
    cwByAngle(motion, normalizeAngle(aimAngle - shipAngle), 0.1f);
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

#include <chrono>
#include <iostream>

namespace {

template <typename Vec>
struct Node {
  Vec pos;
  Vec prev;
  float steps;
  float cost;
};

template <typename Vec>
struct CompareNodes {
  bool operator()(const Node<Vec> &lhs, const Node<Vec> &rhs) const noexcept {
    return lhs.cost > rhs.cost;
  }
};

template <typename Vec>
struct NodeQueue : std::priority_queue<Node<Vec>, std::vector<Node<Vec>>, CompareNodes<Vec>> {
  using NodeQueue::priority_queue::c;
};

/*
struct Policy {
  // a type that represents a 2D position
  // should support comparison and addition
  using Vec = ...

  // how many steps does it take to move in this direction?
  float steps(Vec) const;
  // what is the distance between these points?
  float distance(Vec, Vec) const;
  // what is the cost for turning from one direction to another?
  float turn(Vec, Vec) const;
  // is it possible to walk to this point by walking in this direction?
  bool walkable(Vec, Vec) const;
  // get a range of Vecs pointing at the neighbors
  range<Vec> neighbors() const;
  
  // this node is now being processed
  // (makes it possible to get list of points that make up the path)
  void next(const Node &);
  
  // no path was found
  void fail();
  // a path was found
  void succeed(Node &);
};
*/

template <typename Policy>
void astar(Policy &policy, const typename Policy::Vec from, const typename Policy::Vec to) {
  // TODO: this function never returns when there is no route
  
  NodeQueue<typename Policy::Vec> queue;
  queue.push({to, to, 0, policy.distance(from, to)});
  
  while (true) {
    if (queue.empty()) {
      policy.fail();
      return;
    } else if (queue.top().pos == from) {
      policy.succeed(queue.top());
      return;
    }
    
    const Node top = queue.top();
    queue.pop();
    policy.next(top);
    
    for (const auto dir : policy.neighbors()) {
      const auto neighborPos = top.pos + dir;
      if (neighborPos == top.prev) continue;
      if (!policy.walkable(neighborPos, dir)) continue;
      
      const float neighborSteps = top.steps + policy.steps(dir);
      const float neighborDist = policy.distance(neighborPos, from);
      const float neighborTurn = policy.turn(top.pos - top.prev, neighborPos - top.pos);
      const float neighborCost = neighborSteps + neighborDist + neighborTurn;
      
      // http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1021r2.html
      const Node<typename Policy::Vec> neighbor = {
        neighborPos,
        top.pos,
        neighborSteps,
        neighborCost
      };
      bool found = false;
      
      for (auto n = queue.c.begin(); n != queue.c.end(); ++n) {
        if (n->pos == neighborPos) {
          found = true;
          if (neighbor.steps < n->steps) {
            // TODO: Remove n from the queue
            queue.push(neighbor);
          }
          break;
        }
      }
      
      if (!found) {
        queue.push(neighbor);
      }
    }
  }
}

using NextPos = std::array<b2Vec2, 2>;

class Policy {
public:
  using Vec = b2Vec2;
  using Node = Node<Vec>;

  static float steps(const Vec dir) {
    return dir.Length();
  }
  static float distance(const Vec a, const Vec b) {
    return b2Distance(a, b);
  }
  static float turn(const Vec a, const Vec b) {
    return std::acos(b2Dot(normalized(a), normalized(b))) * 0.05f;
  }
  
  bool wall(const Vec pos) const {
    if (pos.x < 0 || pos.y < 0) return true;
    if (pos.x >= map.width || pos.y >= map.height) return true;
    return map.data[pos.y * map.width + pos.x];
  }
  bool walkable(const Vec to, const Vec dir) const {
    if (b2Abs(dir) == b2Vec2{1.0f, 1.0f}) {
      if (wall({to.x - dir.x, to.y}) || wall({to.x, to.y - dir.y})) return false;
    }
    return !wall(to);
  }
  
  static inline const b2Vec2 neighborArray[] = {
    {0.0f, -1.0f},
    {1.0f, -1.0f},
    {1.0f, 0.0f},
    {1.0f, 1.0f},
    {0.0f, 1.0f},
    {-1.0f, 1.0f},
    {-1.0f, 0.0f},
    {-1.0f, -1.0f}
  };
  
  static const auto &neighbors() {
    return neighborArray;
  }
  
  void next(const Node &node) {
    nodes.push_back(node);
  }
  
  void fail() {
    assert(false);
  }
  void succeed(const Node &node) {
    std::cout << "Path length: " << node.steps << '\n';
    nodes.push_back(node);
  }
  
  explicit Policy(const MapData &map)
    : map{map} {}
  
  void init() {
    nodes.clear();
  }
  
  template <typename Func>
  void eachPosition(Func func) const {
    b2Vec2 prev = nodes.back().prev;
    for (auto n = nodes.crbegin(); n != nodes.crend(); ++n) {
      const auto &node = *n;
      if (node.pos == prev) {
        func(node.pos);
        prev = node.prev;
      }
    }
  }

private:
  const MapData &map;
  std::vector<Node> nodes;
};

float angleBetween(const b2Vec2 a, const b2Vec2 b) {
  return std::acos(b2Dot(normalized(a), normalized(b)));
}

using Clock = std::chrono::system_clock;
Clock::time_point start, end;
bool done = false;

float getSeconds(Clock::duration duration) {
  const long millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
  return std::round(millis / 100.0f) / 10.f;
}

}

void behaveNavigate(entt::registry &reg) {
  const MapData &map = reg.ctx<MapData>();
  Policy policy{map};
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
  
  entt::each(reg, [&](Physics phys, MotionCommand &motion, MotionParams params, NavigateBehaviour &behave) {
    const b2Vec2 shipPos = phys.body->GetPosition();
    
    if (behave.path.empty()) {
      const b2Vec2 fromPos = tilePos(shipPos);
      const b2Vec2 toPos = tilePos({behave.x, behave.y});
      if (fromPos == toPos) {
        behave.path.push_back(floorPos({behave.x, behave.y}));
      } else {
        policy.init();
        astar(policy, fromPos, toPos);
        policy.eachPosition([&](const b2Vec2 pos) {
          behave.path.push_back(worldPos(pos));
        });
      }
      start = Clock::now();
    }
    
    if (behave.path.size() > 1) {
      if (b2DistanceSquared(shipPos, behave.path[0]) < 4.0f) {
        behave.path.erase(behave.path.begin());
      } else if (b2DistanceSquared(shipPos, behave.path[1]) < 36.0f) {
        behave.path.erase(behave.path.begin());
      }
    }
    
    b2Vec2 desiredVel = 2.0f * (behave.path[0] - shipPos);
    if (behave.path.size() > 1) {
      desiredVel = behave.path[0] + behave.path[1] - 2.0f * shipPos;
      const float angle = angleBetween(behave.path[1] - behave.path[0], behave.path[0] - shipPos);
      if (angle < b2_pi / 3.0f) {
        desiredVel = scaleToLength(desiredVel, 1.05f * params.speed);
      }
    } else {
      if (!std::exchange(done, true)) {
        end = Clock::now();
        std::cout << "Travel time: " << getSeconds(end - start) << " seconds\n";
      }
    }
    
    // TODO: Remove these
    if (behave.debug0 != entt::null) {
      reg.get<Physics>(behave.debug0).body->SetTransform(behave.path[0], 0.0f);
    }
    if (behave.debug1 != entt::null) {
      const b2Vec2 point = behave.path.size() > 1 ? behave.path[1] : behave.path[0];
      reg.get<Physics>(behave.debug1).body->SetTransform(point, 0.0f);
    }
    
    const b2Vec2 accel = desiredVel - phys.body->GetLinearVelocity();
    const b2Vec2 forwardDir = angleMag(phys.body->GetAngle(), 1.0f);
    const b2Vec2 rightDir = forwardDir.Skew();
    const float aimAngle = std::atan2(desiredVel.y, desiredVel.x);
    
    forwardByAccel(motion, b2Dot(accel, forwardDir));
    rightByAccel(motion, b2Dot(accel, rightDir));
    cwByAngle(motion, normalizeAngle(aimAngle - phys.body->GetAngle()));
  });
}
