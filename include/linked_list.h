/* ./cstruct/include/linked_list.h */

#ifndef CSTRUCT_LINKED_LIST_H
#define CSTRUCT_LINKED_LIST_H

#include <stddef.h>

#include "error.h"

typedef struct Node Node;

typedef struct LinkedList {
    Node* head;         // Pointer to the first node of the linked list
    Node* tail;         // Pointer to the last node of the linked list
    size_t size;        // Size of the linked list
} LinkedList;

ErrorCode linked_list_init(LinkedList* list);
ErrorCode linked_list_insert_at_beginning(LinkedList* list, const void* data);
ErrorCode linked_list_insert_at_end(LinkedList* list, const void* data);
ErrorCode linked_list_insert_at_position(LinkedList* list, const void* data, size_t index);
ErrorCode linked_list_delete_at_beginning(LinkedList* list);
ErrorCode linked_list_delete_at_end(LinkedList* list);
ErrorCode linked_list_delete_at_position(LinkedList* list, size_t index);

void* linked_list_search_by_pointer(const LinkedList* list, const void* value);
void* linked_list_search_by_position(const LinkedList* list, size_t index);

void linked_list_traverse(const LinkedList* list, void (*visit)(void* data, void* context), void* context);
void linked_list_free(LinkedList* list);

#endif /* CSTRUCT_LINKED_LIST_H */
