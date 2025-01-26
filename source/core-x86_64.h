#ifndef CORE_X86_64_I
#define CORE_X86_64_I

#include "core.h"

#include <assert.h>
#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// Invoke `task(x)` on entrance to a core with `core_switch(core, x)`
void entrance_hook(void);

// Call `core_return(x)` on a `return x` in a task
void return_hook(void);

// Just a "ret" to maintain stack alignment
void bounce_hook(void);

// Terminate on a `core_switch(...)` into a core with a returned task
static void zombie_hook(void);

__attribute__((used))
static inline estate_t estate_initialize(
    void *(*task)(void *), size_t size, char allocation[size]) {
  _Static_assert(sizeof(void *) == 8, "sizeof(void *) != 8");
  _Static_assert(alignof(void *) == 8, "alignof(void *) != 8");

  // Maintain stack alignment. In the x86-64 ABI, the stack must be aligned to
  // 16 *before* the call instruction, i.e. on entrance to a function, the stack
  // must be aligned to 16, shifted by 8.
  size &= ~(16 - 1);
  if (__builtin_expect(size < sizeof(void *[64]), 0))
    return NULL;

  void **tail = (void **) &allocation[size];

  tail[-2] = zombie_hook;   // Must be even for stack alignment
  tail[-3] = bounce_hook;
  tail[-5] = return_hook;
  tail[-6] = task;          // Must be even for stack alignment
  tail[-7] = entrance_hook;

  // This must match the changes done to rsp before it's saved
  return &tail[-13];
}

__attribute__((cold, noreturn))
static void zombie_hook(void) {
  fputs("Failed to switch into exited core\n", stderr);
  abort();
}

#endif /* CORE_X86_64_I */
