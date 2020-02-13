//
//  frame_cap.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 13/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef utils_frame_cap_hpp
#define utils_frame_cap_hpp

#include <cstdint>

/*
class FrameCap {
public:
  using Clock = std::chrono::steady_clock;
  
  explicit FrameCap(const int fps)
    : interval{Clock::duration{Clock::duration::period::den / fps}},
      start{Clock::now()} {}
      
  ~FrameCap() {
    std::this_thread::sleep_until(start + interval);
  }

private:
  const Clock::duration interval;
  const Clock::time_point start;
};
*/

class FrameCap {
public:
  explicit FrameCap(int);
  void begin();
  void end();

private:
  std::uint64_t lowerFrameTime;
  std::uint64_t targetFrameTime;
  std::uint64_t toMilli;
  std::uint64_t frameStart;
};

#endif
