/* ./cstruct/include/list.h */

#ifndef CSTRUCT_LIST_H
#define CSTRUCT_LIST_H

#include <stddef.h>

#include "error.h"

typedef struct Node Node;

typedef struct List {
    Node*   head;   // Pointer to the first node of the linked list
    Node*   tail;   // Pointer to the last node of the linked list
    size_t  size;   // Size of the linked list
} List;

ErrorCode list_init(List* list);
ErrorCode list_insert_at_beginning(List* list, const void* data);
ErrorCode list_insert_at_end(List* list, const void* data);
ErrorCode list_insert_at_position(List* list, const void* data, size_t index);
ErrorCode list_delete_at_beginning(List* list);
ErrorCode list_delete_at_end(List* list);
ErrorCode list_delete_at_position(List* list, size_t index);

void* list_get_head(const List* list);
void* list_get_tail(const List* list);
void* list_search_by_pointer(const List* list, const void* value);
void* list_search_by_position(const List* list, size_t index);

void list_traverse(const List* list, void (*visit)(void* data, void* context), void* context);
void list_free(List* list);

#endif /* CSTRUCT_LIST_H */
