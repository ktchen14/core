#ifndef CORE_H
#define CORE_H

#include <stddef.h>

/// An architecture-specific data structure to resume a core
typedef void *estate_t;

typedef struct core_t core_t;
struct core_t {
  _Alignas(max_align_t) estate_t estate;

  /// The core that this core should core_return() to, or @c NULL
  core_t *return_to;
};

/// The active core in the thread
extern _Thread_local core_t *core_active;

/// The main core in the thread
extern _Thread_local core_t core_main;

/// This must be called in a thread when it's created
void core_initialize_thread(void);

/**
 * @brief Initialize the @a core to call @a task with the @a allocation
 *
 * The @a allocation should be an allocated object. It's used to hold the stack
 * of the core.
 *
 * The behavior is undefined if:
 * - @a core, @a task, or @a allocation is @c NULL
 * - @a allocation isn't aligned to a max_align_t
 *
 * @param core the core to initialize
 * @param task the task to call when the @a core is switched into
 * @param allocation an allocated object
 * @param size the size of the @a allocation
 * @return @a core on success; otherwise @c NULL
 */
core_t *core_initialize(
  core_t *core, void *(*task)(void *), void *allocation, size_t size
) __attribute__((nonnull));

/// Switch to the @a next core with argument @a result
void *core_switch(core_t *next, void *result)
  __attribute__((nonnull(1)));

/**
 * @brief Invoke the core @a next with argument @a result
 *
 * If @a next had previously been resumed, and hasn't yet done a core_return(),
 * then the behavior is undefined.
 *
 * The root core must not be resumed.
 */
void *core_resume(core_t *next, void *result)
  __attribute__((nonnull(1)));

/// Return to the core that resumed this one with return value @a result
void *core_return(void *result);

#endif /* CORE_H */
