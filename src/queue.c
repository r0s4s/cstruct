/* ./cstruct/src/queue.c */

#include "queue.h"

ErrorCode queue_init(Queue* queue) {
    // Check for invalid parameter value
    if (queue == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize the linked list to hold the queue elements
    ErrorCode init_code = list_init(&queue->data);
    if (init_code != CSTRUCT_SUCCESS) {
        // Initialization failed
        return init_code;
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode queue_enqueue(Queue* queue, const void* value) {
    // Check for invalid parameter values
    if (queue == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Enqueue the value at the end of the queue
    ErrorCode insert_code = list_insert_back(&queue->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        // Insertion failed
        return insert_code;
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
    ErrorCode delete_code = list_delete_front(&queue->data);
    if (delete_code != CSTRUCT_SUCCESS) {
        // Deletion failed
        return delete_code;
    }

    return CSTRUCT_SUCCESS;
}

void* queue_peek(const Queue* queue) {
    // Check for invalid parameter values
    if (queue == NULL || queue->data.size == 0) {
        return NULL;
    }
    
    // Directly return the head of the queue data linked list
    return list_get_head(&queue->data);
}

void queue_free(Queue* queue) {
    // Check for invalid parameter value
    if (queue == NULL) {
        return;
    }
    
    // Free the queue linked list
    list_free(&queue->data);
}
