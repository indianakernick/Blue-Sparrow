//
//  scope time.hpp
//  Blue Sparrow
//
//  Created by Indiana Kernick on 1/12/19.
//  Copyright © 2019 Indiana Kernick. All rights reserved.
//

#ifndef utils_scope_time_hpp
#define utils_scope_time_hpp

#ifdef ENABLE_SCOPE_TIME

#include <chrono>
#include <memory>
#include <cassert>

class ScopeTime {
public:
  explicit ScopeTime(const char *);
  ~ScopeTime();
  
  static void print();
  static void reset();
  
private:
  using Clock = std::chrono::high_resolution_clock;

  Clock::time_point start;
  
  void enterScope(const char *);

  struct TreeNode {
    std::size_t calls = 0;
    Clock::duration time;
    const char *name;
    TreeNode *parent;
    std::unique_ptr<TreeNode[]> children;
    std::size_t childCount = 0;
  };
  
  static constexpr std::size_t max_child_count = 32;
  static inline TreeNode tree {0, {}, "ROOT", nullptr, {}, 0};
  static inline TreeNode *current = &tree;
  
  static void printImpl(const TreeNode *, int);
  
  static constexpr int name_indent = 2;
  static constexpr int num_prec = 4;
  static constexpr int name_width = 48;
  static constexpr int rest_width = 24;
};

inline ScopeTime::ScopeTime(const char *name) {
  enterScope(name);
  start = Clock::now();
}

inline ScopeTime::~ScopeTime() {
  current->time += Clock::now() - start;
  ++current->calls;
  current = current->parent;
}

#define SCOPE_TIME_IMPL2(NAME, LINE) ScopeTime scope_time_##LINE {NAME}
#define SCOPE_TIME_IMPL(NAME, LINE) SCOPE_TIME_IMPL2(NAME, LINE)
#define SCOPE_TIME(NAME) SCOPE_TIME_IMPL(NAME, __LINE__)
#define NO_SCOPE_TIME(NAME) SCOPE_TIME(nullptr)
#define SCOPE_TIME_PRINT() ScopeTime::print()
#define SCOPE_TIME_RESET() ScopeTime::reset()

#else

#define SCOPE_TIME(NAME)
#define NO_SCOPE_TIME(NAME)
#define SCOPE_TIME_PRINT()
#define SCOPE_TIME_RESET()

#endif

#endif
