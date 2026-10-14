/* ./cstruct/src/deque.c */

#include "deque.h"

ErrorCode deque_init(Deque* deque) {
    // Check for invalid parameter value
    if (deque == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize the linked list to hold the deque's elements
    ErrorCode init_code = linked_list_init(&deque->data);
    if (init_code != CSTRUCT_SUCCESS) {
        return init_code; // Initialization failed
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode deque_insert_front(Deque* deque, const void* value) {
    // Check for invalid parameter values
    if (deque == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Insert the value at the front of the deque
    ErrorCode insert_code = linked_list_insert_at_beginning(&deque->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        return insert_code; // Insertion failed
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode deque_insert_back(Deque* deque, const void* value) {
    // Check for invalid parameter values
    if (deque == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Insert the value at the end of the deque
    ErrorCode insert_code = linked_list_insert_at_end(&deque->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        return insert_code; // Insertion failed
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

    // Delete the value at the beginning of the deque
    ErrorCode delete_code = linked_list_delete_at_beginning(&deque->data);
    if (delete_code != CSTRUCT_SUCCESS) {
        return delete_code; // Deletion failed
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

    // Delete the value at the end of the deque
    ErrorCode delete_code = linked_list_delete_at_end(&deque->data);
    if (delete_code != CSTRUCT_SUCCESS) {
        return delete_code; // Deletion failed
    }

    return CSTRUCT_SUCCESS; 
}

void* deque_peek_front(const Deque* deque) {
    // Check for invalid parameter values
    if (deque == NULL || deque->data.size == 0) {
        return NULL;
    }
    
    // Directly return the head of the deque's data linked list
    return linked_list_get_head(&deque->data);
}

void* deque_peek_back(const Deque* deque) {
    // Check for invalid parameter values
    if (deque == NULL || deque->data.size == 0) {
        return NULL;
    }
    
    // Directly return the tail of the deque's data linked list
    return linked_list_get_tail(&deque->data);
}

void deque_free(Deque* deque) {
    // Check for invalid parameter value
    if (deque == NULL) {
        return;
    }
    
    // Free the deque's linked list
    linked_list_free(&deque->data);
}
