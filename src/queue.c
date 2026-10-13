/* ./cstruct/src/queue.c */

#include "queue.h"

ErrorCode queue_init(Queue* queue) {
    // Check for invalid parameter value
    if (queue == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize the linked list to hold the queue's elements
    ErrorCode init_code = linked_list_init(&queue->data);
    if (init_code != CSTRUCT_SUCCESS) {
        return init_code; // Initialization failed
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode queue_enqueue(Queue* queue, const void* value) {
    // Check for invalid parameter values
    if (queue == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Enqueue the value at the end of the queue
    ErrorCode insert_code = linked_list_insert_at_end(&queue->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        return insert_code; // Insertion failed
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode queue_dequeue(Queue* queue) {
    // Check for invalid parameter value
    if (queue == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Underflow error if queue is empty
    if (queue->data.size == 0) {
        return CSTRUCT_ERR_UNDERFLOW;
    }

    // Dequeue the value at the beginning of the queue
    ErrorCode delete_code = linked_list_delete_at_beginning(&queue->data);
    if (delete_code != CSTRUCT_SUCCESS) {
        return delete_code; // Deletion failed
    }

    return CSTRUCT_SUCCESS;
}

void* queue_peek(const Queue* queue) {
    // Check for invalid parameter values
    if (queue == NULL || queue->data.size == 0) {
        return NULL;
    }
    
    // Directly return the head of the queue's data linked list
    return linked_list_get_head(&queue->data);
}

void queue_free(Queue* queue) {
    // Check for invalid parameter value
    if (queue == NULL) {
        return;
    }
    
    // Free the queue's linked list
    linked_list_free(&queue->data);
}
