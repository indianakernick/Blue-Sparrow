//
//  sdl_check.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 6/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef utils_sdl_check_hpp
#define utils_sdl_check_hpp

#include <stdexcept>
#include <SDL_error.h>

namespace SDL {

class Error final : public std::runtime_error {
public:
  explicit Error(const char *error) noexcept
    : std::runtime_error{error} {}
};

[[noreturn]] inline void raise() {
  const char *error = SDL_GetError();
  throw Error{error};
}

template <typename Ptr>
[[nodiscard]] Ptr *check(Ptr *ptr) {
  if (ptr == nullptr) raise();
  return ptr;
}

inline int check(const int code) {
  if (code < 0) raise();
  return code;
}

}

#ifdef DISABLE_SDL_ERROR_CHECK
#  define SDL_CHECK(EXPR) EXPR
#else
#  define SDL_CHECK(EXPR) ::SDL::check(EXPR)
#endif

#endif
