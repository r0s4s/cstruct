/* ./cstruct/include/queue.h */

#ifndef CSTRUCT_QUEUE_H
#define CSTRUCT_QUEUE_H

#include <stddef.h>

#include "error.h"
#include "list.h"

typedef struct Queue {
    List data; // Linked list to hold the queue elements
} Queue;

ErrorCode queue_init(Queue* queue);
ErrorCode queue_enqueue(Queue* queue, const void* value);
ErrorCode queue_dequeue(Queue* queue);

void* queue_peek(const Queue* queue);
void queue_free(Queue* queue);

#endif /* CSTRUCT_QUEUE_H */
