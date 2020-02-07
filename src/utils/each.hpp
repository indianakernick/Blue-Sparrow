//
//  each.hpp
//  Blue Sparrow
//
//  Created by Indi Kernick on 7/2/20.
//  Copyright © 2020 Indiana Kernick. All rights reserved.
//

#ifndef utils_each_hpp
#define utils_each_hpp

#include <entt/entity/registry.hpp>

namespace entt {

namespace internal {

template <typename First, typename... Rest, typename Entity>
auto view_from_list(basic_registry<Entity> &reg) {
  if constexpr (std::is_same_v<First, Entity>) {
    return reg.view<Rest...>();
  } else {
    return reg.view<First, Rest...>();
  }
}

template <typename Entity, typename Ret, typename Class, typename... Args>
auto view_from_args(basic_registry<Entity> &reg, Ret (Class::*)(Args...)) {
  return view_from_list<std::decay_t<Args>...>(reg);
}

template <typename Entity, typename Ret, typename Class, typename... Args>
auto view_from_args(basic_registry<Entity> &reg, Ret (Class::*)(Args...) const) {
  return view_from_list<std::decay_t<Args>...>(reg);
}

template <typename Entity, typename Ret, typename... Args>
auto view_from_args(basic_registry<Entity> &reg, Ret (*)(Args...)) {
  return view_from_list<std::decay_t<Args>...>(reg);
}

template <typename Entity, typename Func>
auto view_from_func(basic_registry<Entity> &reg, Func, choice_t<1>)
  -> decltype(view_from_args(reg, &Func::operator())) {
  return view_from_args(reg, &Func::operator());
}

template <typename Entity, typename Func>
auto view_from_func(basic_registry<Entity> &reg, Func func, choice_t<0>) {
  return view_from_args(reg, func);
}

}

template <typename Entity, typename Func>
void each(basic_registry<Entity> &reg, Func func) {
  internal::view_from_func(reg, func, choice<1>).each(func);
}

}

#endif
