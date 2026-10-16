/* ./cstruct/src/dlist.c */

#include <stdlib.h>

#include "dlist.h"

typedef struct DNode {
    void*           data;   // Generic data pointer
    struct DNode*   next;   // Pointer to the next node
    struct DNode*   prev;   // Pointer to the previous node
} DNode;

ErrorCode dlist_init(DList* dlist) {
    // Check for invalid parameter value
    if (dlist == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize doubly linked list
    dlist->head = NULL;
    dlist->tail = NULL;
    dlist->size = 0;
    
    return CSTRUCT_SUCCESS;
}

ErrorCode dlist_insert_front(DList* dlist, const void* data) {
    // Check for invalid parameter values
    if (dlist == NULL || data == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Create a new node
    DNode* new_node = malloc(sizeof(DNode));
    if (new_node == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    new_node->data = (void*)data;
    new_node->next = dlist->head;
    new_node->prev = NULL;

    // Update the doubly linked list
    if (dlist->size > 0) {
        // Only update the old head previous pointer if the list is not empty
        dlist->head->prev = new_node;
    } else {
        // If the list is empty the new node is also the tail
        dlist->tail = new_node;
    }

    // Update head pointer
    dlist->head = new_node;
    
    // Increment the size of the doubly linked list
    dlist->size++;

    return CSTRUCT_SUCCESS;
}

ErrorCode dlist_insert_back(DList* dlist, const void* data) {
    // Check for invalid parameter values
    if (dlist == NULL || data == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Create a new node
    DNode* new_node = malloc(sizeof(DNode));
    if (new_node == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    new_node->data = (void*)data;
    new_node->next = NULL;
    new_node->prev = dlist->tail;

    // Update the doubly linked list
    if (dlist->size == 0) {
        // If the doubly linked list is empty the new node is also the first and last
        dlist->head = new_node;
        dlist->tail = new_node;
    } else {
        // Otherwise set the next pointer of the current tail to the new node
        dlist->tail->next = new_node;
        dlist->tail = new_node;
    }

    // Increment the size of the doubly linked list
    dlist->size++;

    return CSTRUCT_SUCCESS;
}

ErrorCode dlist_insert_at(DList* dlist, const void* data, size_t index) {
    // Check for invalid parameter values
    if (dlist == NULL || data == NULL || index > dlist->size) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Create a new node
    DNode* new_node = malloc(sizeof(DNode));
    if (new_node == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    new_node->data = (void*)data;
    new_node->next = NULL;
    new_node->prev = NULL;

    // Handle edge cases
    if (dlist->size == 0) {
        // If the doubly linked list is empty the new node is also the first and last
        dlist->head = new_node;
        dlist->tail = new_node;
    } else if (index == 0) {
        // If the index is 0 the new node is at the beginning
        new_node->next = dlist->head;
        dlist->head->prev = new_node;
        dlist->head = new_node;
    } else if (index == dlist->size) {
        // If the index is equal to the current size of the doubly linked list,
        // Then the new node is at the end
        new_node->prev = dlist->tail;
        dlist->tail->next = new_node;
        dlist->tail = new_node;
    } else {
        // Traverse the doubly list to find the insertion index
        DNode* current = dlist->head;
        for (size_t i = 0; i < index - 1; i++) {
            current = current->next;
        }

        // Insert the new node after the insertion index
        new_node->next      = current->next;
        new_node->prev      = current;
        current->next->prev = new_node;
        current->next       = new_node;
    }
    
    // Increment the size of the doubly linked list
    dlist->size++;

    return CSTRUCT_SUCCESS;
}

ErrorCode dlist_delete_front(DList* dlist) {
    // Check for invalid parameter values
    if (dlist == NULL || dlist->size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Store the old head
    DNode* old = dlist->head;

    // Update head to the next node
    dlist->head = dlist->head->next;

    // If the doubly list is not empty after deletion,
    // Update the new head previous pointer
    if (dlist->head != NULL) {
        dlist->head->prev = NULL;
    }

    // Free the old head
    free(old);

    // Update the tail if the doubly list becomes empty
    if (dlist->head == NULL) {
        dlist->tail = NULL;
    }

    // Decrement the size of the doubly linked list
    dlist->size--;

    return CSTRUCT_SUCCESS;
}

ErrorCode dlist_delete_back(DList* dlist) {
    // Check for invalid parameter values
    if (dlist == NULL || dlist->size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Update the tail
    if (dlist->head == dlist->tail) {
        // Only one node in the list
        free(dlist->tail);
        dlist->head = NULL;
        dlist->tail = NULL;
    } else {
        DNode* new_tail = dlist->tail->prev;
        free(dlist->tail);
        dlist->tail = new_tail;
        new_tail->next = NULL;
    }

    // Decrement the size of the doubly linked list
    dlist->size--;

    return CSTRUCT_SUCCESS;
}

ErrorCode dlist_delete_at(DList* dlist, size_t index) {
    // Check for invalid parameter values
    if (dlist == NULL || dlist->size == 0 || index >= dlist->size) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Delegate
    if (index == 0) {
        return dlist_delete_front(dlist);
    }

    // Delegate
    if (index == dlist->size - 1) {
        return dlist_delete_back(dlist);
    }

    // Find the node at index traversing from the nearer end
    DNode* to_delete;
    if (index < dlist->size / 2) {
        to_delete = dlist->head;
        for (size_t i = 0; i < index; i++) {
            to_delete = to_delete->next;
        }
    } else {
        to_delete = dlist->tail;
        for (size_t i = 0; i < dlist->size - index - 1; i++) {
            to_delete = to_delete->prev;
        }
    }

    // Unlink it,
    // Both neighbors exist because index 0 and size-1,
    // Were delegated above
    to_delete->prev->next = to_delete->next;
    to_delete->next->prev = to_delete->prev;
    free(to_delete);

    // Decrement the size of the doubly linked list
    dlist->size--;

    return CSTRUCT_SUCCESS;
}

void* dlist_get_head(const DList* dlist) {
    // Check for invalid parameter values
    if (dlist == NULL || dlist->head == NULL) {
        return NULL;
    }

    // Return the doubly list head data
    return dlist->head->data;
}

void* dlist_get_tail(const DList* dlist) {
    // Check for invalid parameter values
    if (dlist == NULL || dlist->tail == NULL) {
        return NULL;
    }

    // Return the doubly list tail data
    return dlist->tail->data;
}

void* dlist_search_by_pointer(const DList* dlist, const void* value) {
    // Check for invalid parameter values
    if (dlist == NULL || value == NULL) {
        return NULL;
    }

    // Traverse the doubly linked list forward
    DNode* current = dlist->head;
    while (current != NULL) {
        if (current->data == value) {
            // Return the data if the pointer matches
            return current->data;
        }

        current = current->next;
    }

    // Return NULL if the pointer is not found
    return NULL;
}

void* dlist_search_by_position(const DList* dlist, size_t index) {
    // Check for invalid parameter values
    if (dlist == NULL || index >= dlist->size) {
        return NULL;
    }

    // Index corresponds to the head
    if (index == 0) {
        return dlist->head->data;
    }

    // Index corresponds to the tail
    if (index == dlist->size - 1) {
        return dlist->tail->data;
    }

    // Determine traversal direction
    if (index < dlist->size / 2) {
        // Traverse from the head
        DNode* current = dlist->head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }

        return current->data;
    } else {
        // Traverse from the tail
        DNode* current = dlist->tail;
        for (size_t i = 0; i < dlist->size - index - 1; i++) {
            current = current->prev;
        }

        return current->data;
    }
}

void dlist_forward(const DList* dlist, void (*visit)(void* data, void* context), void* context) {
    // Check for invalid parameter values
    if (dlist == NULL || visit == NULL) {
        return;
    }

    // Traverse the doubly linked list forward
    DNode* current = dlist->head;
    while (current != NULL) {
        visit(current->data, context);
        current = current->next;
    }
}

void dlist_backward(const DList* dlist, void (*visit)(void* data, void* context), void* context) {
    // Check for invalid parameter values
    if (dlist == NULL || visit == NULL) {
        return;
    }

    // Traverse the doubly linked list backward
    DNode* current = dlist->tail;
    while (current != NULL) {
        visit(current->data, context);
        current = current->prev;
    }   
}

void dlist_free(DList* dlist) {
    // Check for invalid parameter value
    if (dlist == NULL) {
        return;
    }

    // Traverse the doubly linked list forward
    DNode* current = dlist->head;
    while (current != NULL) {
        DNode* next = current->next;
        free(current);
        current = next;
    }

    dlist->head = NULL;
    dlist->tail = NULL;
    dlist->size = 0;
}
