/* ./cstruct/include/deque.h */

#ifndef CSTRUCT_DEQUE_H
#define CSTRUCT_DEQUE_H

#include <stddef.h>

#include "error.h"
#include "linked_list.h"

typedef struct Deque {
    LinkedList data; // Linked list to hold the deque elements
} Deque;

ErrorCode deque_init(Deque* deque);
ErrorCode deque_insert_front(Deque* deque, const void* value);
ErrorCode deque_insert_back(Deque* deque, const void* value);
ErrorCode deque_delete_front(Deque* deque);

// Note: delete_back is O(n) with a singly linked list backing store,
// If O(1) at both ends is required, use a doubly linked list.
ErrorCode deque_delete_back(Deque* deque);

void* deque_peek_front(const Deque* deque);
void* deque_peek_back(const Deque* deque);

void deque_free(Deque* deque);

#endif /* CSTRUCT_DEQUE_H */
