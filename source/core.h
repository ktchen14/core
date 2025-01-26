#ifndef CORE_I
#define CORE_I

#include <core.h>

struct core_t {
  _Alignas(max_align_t) estate_t estate;

  // The core that should resume when this core does core_return(), or @c NULL
  // if no such core exists
  core_t *return_to;
};

extern _Thread_local core_t main_core;

#endif /* CORE_I */
