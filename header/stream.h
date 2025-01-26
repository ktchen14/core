#ifndef CORE_H
#define CORE_H

#include <stddef.h>

typedef struct {
  size_t length;
  size_t volume;
} core_stream_t;

core_stream_t *core_stream_create(size_t volume);

void core_stream_delete(core_stream_t *stream);

size_t core_stream_length(const core_stream_t *stream);

size_t core_stream_volume(const core_stream_t *stream);

core_stream_t *core_stream_enqueue(core_stream_t *stream, const void *data, size_t z);

core_stream_t *core_stream_dequeue(core_stream_t *stream, void *data, size_t z);

#endif /* CORE_H */
