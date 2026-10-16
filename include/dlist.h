/* ./cstruct/include/dlist.h */

#ifndef CSTRUCT_DLIST_H
#define CSTRUCT_DLIST_H

#include <stddef.h>

#include "error.h"

typedef struct DNode DNode;

typedef struct DList {
    DNode*  head;   // Pointer to the first node of the doubly linked list
    DNode*  tail;   // Pointer to the last node of the doubly linked list
    size_t  size;   // Size of the doubly linked list
} DList;

ErrorCode dlist_init(DList* dlist);
ErrorCode dlist_insert_front(DList* dlist, const void* data);
ErrorCode dlist_insert_back(DList* dlist, const void* data);
ErrorCode dlist_insert_at(DList* dlist, const void* data, size_t index);
ErrorCode dlist_delete_front(DList* dlist);
ErrorCode dlist_delete_back(DList* dlist);
ErrorCode dlist_delete_at(DList* dlist, size_t index);

void* dlist_get_head(const DList* dlist);
void* dlist_get_tail(const DList* dlist);
void* dlist_search_by_pointer(const DList* dlist, const void* value);
void* dlist_search_by_position(const DList* dlist, size_t index);

void dlist_forward(const DList* dlist, void (*visit)(void* data, void* context), void* context);
void dlist_backward(const DList* dlist, void (*visit)(void* data, void* context), void* context);
void dlist_free(DList* dlist);

#endif /* CSTRUCT_DLIST_H */
