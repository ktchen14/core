#include "core.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

_Thread_local core_t *core_active;
_Thread_local core_t core_main = {0};

#include "core-x86_64.c"

void *estate_switch(estate_t *target, void *result, estate_t *source)
  __attribute__((nonnull(1, 3)));

core_t *core_initialize(
    core_t *core, void *(*task)(void *), void *allocation, size_t size) {
  estate_t estate;
  if ((estate = estate_initialize(task, size, allocation)) == NULL)
    return NULL;

  *core = (core_t) { .estate = estate };
  return core;
}

__attribute__((used))
core_t *return_active(void) {
  return core_active;
}

void *core_switch(core_t *next, void *result) {
  // Don't test (next == core_active) to skip an uncommon branch

  estate_t *source = &core_active->estate;
  core_active = next;
  return estate_switch(&next->estate, result, source);
}

void *core_resume(core_t *next, void *result) {
  // Can't resume into the "main" core
  assert(next != &core_main);

  // Can't resume into a core that hasn't returned
  assert(core_active->return_to == NULL);

  // Ensure that this isn't a cyclic resume
  for (const core_t *c = core_active; c != NULL; c = c->return_to)
    assert(c != next);

  next->return_to = core_active;
  return core_switch(next, result);
}

void *core_return(void *result) {
  // Can't return from the "main" core
  assert(core_active != &core_main);

  // Can't return without a core to return to
  assert(core_active->return_to != NULL);

  core_t *next = core_active->return_to;
  core_active->return_to = NULL;
  return core_switch(next, result);
}

__attribute__((constructor))
void core_initialize_thread(void) {
  core_active = &core_main;
}
