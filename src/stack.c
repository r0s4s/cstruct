/* ./cstruct/src/stack.c */

#include "stack.h"

ErrorCode stack_init(Stack* stack, size_t capacity, size_t element_size) {
    // Check for invalid parameter values
    if (stack == NULL || capacity == 0 || element_size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Initialize the dynamic array to hold the stack elements
    ErrorCode init_code = dynamic_array_init(&stack->data, capacity, element_size);
    if (init_code != CSTRUCT_SUCCESS) {
        return init_code; // Initialization failed
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode stack_push(Stack* stack, const void* value) {
    // Check for invalid parameter values
    if (stack == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Push the value to the top of the stack
    ErrorCode insert_code = dynamic_array_insert(&stack->data, value);
    if (insert_code != CSTRUCT_SUCCESS) {
        return insert_code; // Insertion failed
    }

    return CSTRUCT_SUCCESS;
}

ErrorCode stack_pop(Stack* stack) {
    // Check for invalid parameter value
    if (stack == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Check for stack underflow
    if (stack->data.size == 0) {
        return CSTRUCT_ERR_UNDERFLOW;
    }

    // Pop the element at the top of stack
    ErrorCode delete_code = dynamic_array_delete(&stack->data, stack->data.size - 1);
    if (delete_code != CSTRUCT_SUCCESS) {
        return delete_code; // Deletion failed
    }

    return CSTRUCT_SUCCESS;
}

void* stack_peek(const Stack* stack) {
    // Check for invalid parameter value
    if (stack == NULL || stack->data.size == 0) {
        return NULL;
    }
    
    // Directly return the element at the top of the stack
    // If NULL is returned, it is assumed that the call failed
    return dynamic_array_get(&stack->data, stack->data.size - 1);
}

void stack_free(Stack* stack) {
    // Check for invalid parameter value
    if (stack == NULL) {
        return;
    }
    
    // Free the stack's dynamic array
    dynamic_array_free(&stack->data);
}
