/* ./cstruct/src/linked_list.c */

#include <stdlib.h>

#include "linked_list.h"

typedef struct Node {
    void* data;         // Generic data pointer
    struct Node* next;  // Pointer to the next node
} Node;

ErrorCode linked_list_init(LinkedList* list) {
    // Check for invalid parameter value
    if (list == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize linked list
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    
    return CSTRUCT_SUCCESS;
}

ErrorCode linked_list_insert_at_beginning(LinkedList* list, const void* data) {
    // Check for invalid parameter values
    if (list == NULL || data == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Create a new node
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    new_node->data = (void*)data;
    new_node->next = list->head;

    // Update the linked list
    list->head = new_node;
    if (list->size == 0) {
        // If the list was empty the new node is also the first and last
        list->tail = new_node;
    }

    // Increment the size of the linked list
    list->size++;
    return CSTRUCT_SUCCESS;
}

ErrorCode linked_list_insert_at_end(LinkedList* list, const void* data) {
    // Check for invalid parameter values
    if (list == NULL || data == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Create a new node
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    new_node->data = (void*)data;
    new_node->next = NULL;

    // Update the linked list
    if (list->size == 0) {
        // If the linked list is empty, the new node is also the first and last
        list->head = new_node;
        list->tail = new_node;
    } else {
        // Otherwise, set the next pointer of the current tail to the new node
        list->tail->next = new_node;
        list->tail = new_node;
    }

    // Increment linked list size
    list->size++;
    return CSTRUCT_SUCCESS;
}

ErrorCode linked_list_insert_at_position(LinkedList* list, const void* data, size_t index) {
    // Check for invalid parameter values
    if (list == NULL || data == NULL || index > list->size) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Create a new node
    Node* new_node = malloc(sizeof(Node));
    if (new_node == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    new_node->data = (void*)data;
    new_node->next = NULL;

    // Handle edge cases
    if (list->size == 0) {
        // If the linked list is empty, the new node is also the first and last
        list->head = new_node;
        list->tail = new_node;
    } else if (index == 0) {
        // If the index is 0, the new node is at the beginning
        new_node->next = list->head;
        list->head = new_node;
    } else if (index == list->size) {
        // If the index is equal to the current size of the linked list,
        // Then the new node is at the end
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        // Traverse the list to find the insertion index
        Node* current = list->head;
        for (size_t i = 0; i < index - 1; i++) {
            current = current->next;
        }

        // Insert the new node after the insertion index
        new_node->next = current->next;
        current->next = new_node;
    }
    
    // Increment linked list size
    list->size++;
    return CSTRUCT_SUCCESS;
}

ErrorCode linked_list_delete_at_beginning(LinkedList* list) {
    // Check for invalid parameter values
    if (list == NULL || list->size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Store the old head
    Node* old = list->head;

    // New head
    list->head = list->head->next;

    // Free and clear the previous head
    free(old);

    // Update the tail if the list becomes empty
    if (list->head == NULL) {
        list->tail = NULL;
    }

    // Decrement the size of the linked list
    list->size--;
    return CSTRUCT_SUCCESS;
}

ErrorCode linked_list_delete_at_end(LinkedList* list) {
    // Check for invalid parameter values
    if (list == NULL || list->size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Update the tail
    if (list->head == list->tail) {
        // Only one node in the list
        free(list->tail);
        list->head = NULL;
        list->tail = NULL;
    } else {
        Node* current = list->head;
        while (current->next != list->tail) {
            current = current->next;
        }

        free(list->tail);
        list->tail = current;
        current->next = NULL;
    }

    // Decrement the size of the linked list
    list->size--;
    return CSTRUCT_SUCCESS;
}

ErrorCode linked_list_delete_at_position(LinkedList* list, size_t index) {
    // Check for invalid parameter values
    if (list == NULL || list->size == 0 || index >= list->size) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Delegate
    if (index == 0) {
        return linked_list_delete_at_beginning(list);
    }

    // Delegate
    if (index == list->size - 1) {
        return linked_list_delete_at_end(list);
    }

    // Traverse the linked list to find the node to delete
    Node* previous = list->head;
    for (size_t i = 0; i < index - 1; i++) {
        previous = previous->next;
    }

    Node* to_delete = previous->next;
    previous->next = to_delete->next;
    free(to_delete);

    // Decrement the size of the linked list
    list->size--;
    return CSTRUCT_SUCCESS;
}

void* linked_list_search_by_pointer(const LinkedList* list, const void* value) {
    // Check for invalid parameter values
    if (list == NULL || value == NULL) {
        return NULL;
    }

    // Traverse the linked list
    Node* current = list->head;
    while (current != NULL) {
        if (current->data == value) {
            return current->data; // Return the node if the value is found
        }

        current = current->next;
    }

    return NULL; // Return NULL if the value is not found
}

void* linked_list_search_by_position(const LinkedList* list, size_t index) {
    // Check for invalid parameter values
    if (list == NULL || index >= list->size) {
        return NULL;
    }
    
    // Traverse the linked list
    Node* current = list->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    return current->data; // Return the node at the specific index
}

void* linked_list_get_head(const LinkedList* list) {
    // Check for invalid parameter values
    if (list == NULL || list->head == NULL) {
        return NULL;
    }

    return list->head->data; // Return the list's head data
}

void linked_list_traverse(const LinkedList* list, void (*visit)(void* data, void* context), void* context) {
    // Check for invalid parameter values
    if (list == NULL || visit == NULL) {
        return;
    }

    // Traverse the linked list
    Node* current = list->head;
    while (current != NULL) {
        visit(current->data, context);
        current = current->next;
    }
}

void linked_list_free(LinkedList* list) {
    // Check for invalid parameter value
    if (list == NULL) {
        return;
    }

    // Traverse the linked list
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}
