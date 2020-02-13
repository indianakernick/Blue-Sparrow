//
//  frame_cap.cpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 13/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#include "frame_cap.hpp"

#include <SDL2/SDL_timer.h>

FrameCap::FrameCap(const int fps) {
  const std::uint64_t freq = SDL_GetPerformanceFrequency();
  lowerFrameTime = freq / (fps + fps / 2);
  targetFrameTime = freq / fps;
  toMilli = freq / 1000;
}

void FrameCap::begin() {
  frameStart = SDL_GetPerformanceCounter();
}

void FrameCap::end() {
  const std::uint64_t frameEnd = SDL_GetPerformanceCounter();
  const std::uint64_t frameLen = frameEnd - frameStart;
  if (frameLen < lowerFrameTime) {
    const std::uint64_t delayTime = (targetFrameTime - frameLen) / toMilli;
    SDL_Delay(static_cast<std::uint32_t>(delayTime));
  }
}
