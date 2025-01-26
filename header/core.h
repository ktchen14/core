#ifndef CORE_H
#define CORE_H

#include <stddef.h>

typedef struct core_t core_t;

/// The active core in the thread
extern _Thread_local core_t *core_active;

/// The main core in the thread
extern _Thread_local core_t *core_main;

/**
 * @brief Initialize the core subsystem within a thread
 *
 * This must be called in each thread before any object or function declared in
 * this header is accessed or called. This is idempotent; it may be called more
 * than once in a thread. It's unnecessary to call this in the main thread of a
 * process.
 */
void core_initialize_thread(void);

/**
 * @brief Create a @a core to call @a task with an allocation of the specified
 *   @a size
 *
 * The behavior is undefined if:
 *   - @a task is @c NULL
 *   - @a size is @c 0
 *
 * @param task the function to call when the returned core is switched into
 * @param size the size of the allocation for the core
 * @return a newly allocated and initialized core on success; otherwise @c NULL
 */
core_t *core_create(void *(*task)(void *), size_t size)
  __attribute__((nonnull));

/**
 * @brief Initialize the @a core to call @a task with the @a allocation
 *
 * The behavior is undefined if:
 *   - @a core, @a task, or @a allocation are @c NULL
 *   - @a allocation isn't aligned to a max_align_t
 *
 * @param core the core to initialize
 * @param task the task to call when the @a core is switched into
 * @param allocation
 * @param size the size of the @a allocation
 * @return @a core on success; otherwise @c NULL
 */
core_t *core_initialize(
  core_t *core, void *(*task)(void *), void *allocation, size_t size
) __attribute__((nonnull));

/// Switch to the @a next core with argument @a result
void *core_switch(core_t *next, void *result) __attribute__((nonnull(1)));

/**
 * @brief Invoke the core @a next with argument @a result
 *
 * If @a next had previously been resumed, and hasn't yet done a core_return(),
 * then the behavior is undefined.
 *
 * The root core must not be resumed.
 */
void *core_resume(core_t *next, void *result) __attribute__((nonnull(1)));

/// Return to the core that resumed this one
void *core_return(void *result);

#endif /* CORE_H */
