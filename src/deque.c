/* ./cstruct/src/deque.c */

#include "deque.h"

ErrorCode deque_init(Deque* deque) {
    // Check for invalid parameter value
    if (deque == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize the doubly linked list to hold the deque elements
    ErrorCode init_code = dlist_init(&deque->data);
    if (init_code != CSTRUCT_SUCCESS) {
        // Initialization failed
        return init_code;
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode deque_insert_front(Deque* deque, const void* value) {
    // Check for invalid parameter values
    if (deque == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Insert the value at the front of the deque
    ErrorCode insert_code = dlist_insert_front(&deque->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        // Insertion failed
        return insert_code;
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode deque_insert_back(Deque* deque, const void* value) {
    // Check for invalid parameter values
    if (deque == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Insert the value at the back of the deque
    ErrorCode insert_code = dlist_insert_back(&deque->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        // Insertion failed
        return insert_code;
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode deque_delete_front(Deque* deque) {
    // Check for invalid parameter value
    if (deque == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Underflow error if deque is empty
    if (deque->data.size == 0) {
        return CSTRUCT_ERR_UNDERFLOW;
    }

    // Delete the value at the front of the deque
    ErrorCode delete_code = dlist_delete_front(&deque->data);
    if (delete_code != CSTRUCT_SUCCESS) {
        // Deletion failed
        return delete_code;
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode deque_delete_back(Deque* deque) {
    // Check for invalid parameter value
    if (deque == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Underflow error if deque is empty
    if (deque->data.size == 0) {
        return CSTRUCT_ERR_UNDERFLOW;
    }

    // Delete the value at the back of the deque (O(1) with a doubly linked list)
    ErrorCode delete_code = dlist_delete_back(&deque->data);
    if (delete_code != CSTRUCT_SUCCESS) {
        // Deletion failed
        return delete_code;
    }

    return CSTRUCT_SUCCESS;
}

void* deque_peek_front(const Deque* deque) {
    // Check for invalid parameter values
    if (deque == NULL || deque->data.size == 0) {
        return NULL;
    }

    // Directly return the head of the deque data doubly linked list
    return dlist_get_head(&deque->data);
}

void* deque_peek_back(const Deque* deque) {
    // Check for invalid parameter values
    if (deque == NULL || deque->data.size == 0) {
        return NULL;
    }

    // Directly return the tail of the deque data doubly linked list
    return dlist_get_tail(&deque->data);
}

void deque_free(Deque* deque) {
    // Check for invalid parameter value
    if (deque == NULL) {
        return;
    }

    // Free the deque doubly linked list
    dlist_free(&deque->data);
}
