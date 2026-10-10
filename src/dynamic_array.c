/* ./cstruct/src/dynamic_array.c */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../include/dynamic_array.h"

ErrorCode dynamic_array_init(DynamicArray *array, size_t capacity, size_t element_size) {
    // Check for invalid parameter values
    if (array == NULL || capacity == 0 || element_size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Prevent capacity * element_size from overflowing size_t
    if (capacity > SIZE_MAX / element_size) {
        return CSTRUCT_ERR_OVERFLOW;
    }

    // Allocate new data based on the desired capacity
    void *new_data = malloc(capacity * element_size);
    if (new_data == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    // Initialize the dynamic array
    array->data = new_data;
    array->size = 0;
    array->capacity = capacity;
    array->element_size = element_size;

    return CSTRUCT_SUCCESS;
}

ErrorCode dynamic_array_resize(DynamicArray *array, size_t new_capacity) {
    // Check for invalid parameter values
    if (array == NULL || array->element_size == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Check if the new capacity is lower or equal to the current
    if (new_capacity <= array->capacity) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Prevent new_capacity * array->element_size from overflowing size_t
    if (new_capacity > SIZE_MAX / array->element_size) {
        return CSTRUCT_ERR_OVERFLOW;
    }

    // Allocate new data based on the desired capacity
    void *new_data = malloc(new_capacity * array->element_size);
    if (new_data == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    // Copy only the live elements and not the whole old capacity
    memcpy(new_data, array->data, array->size * array->element_size);

    // Commit new state then release old storage
    free(array->data);
    array->data     = new_data;
    array->capacity = new_capacity;

    return CSTRUCT_SUCCESS;
}

ErrorCode dynamic_array_insert(DynamicArray *array, const void *new_data) {
    // Check for invalid parameter values
    if (array == NULL || new_data == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Check if doubling the array size would overflow
    if (array->size >= array->capacity) {
        if (array->capacity > SIZE_MAX / 2) {
            return CSTRUCT_ERR_OVERFLOW;
        }

        ErrorCode resize_code = dynamic_array_resize(array, array->capacity * 2);
        if (resize_code != CSTRUCT_SUCCESS) {
            return resize_code; // Propagate the real error code from the resize call
        }
    }

    // Compute the new_data address
    void *insertion_address = (char *)array->data + (array->size * array->element_size);

    // Copy the new_data into the array
    memcpy(insertion_address, new_data, array->element_size);

    // Update the size of the array
    array->size++;
    return CSTRUCT_SUCCESS;
}

ErrorCode dynamic_array_delete(DynamicArray* array, size_t index) {
    // Check for invalid parameter values
    if (array == NULL || index >= array->size) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Shift elements of the data array to fill the gap
    memmove(
        (char *)array->data + index * array->element_size,
        (char *)array->data + (index + 1) * array->element_size,
        (array->size - index - 1) * array->element_size
    );

    // Update the size of the data array
    array->size--;
    return CSTRUCT_SUCCESS;
}

void* dynamic_array_get(const DynamicArray* array, size_t index) {
    // Check for invalid parameter values
    if (array == NULL || index >= array->size) {
        return NULL; // Because this function returns void*
    }

    // Get the desired value at index
    return (char *)array->data + index * array->element_size;
}

void dynamic_array_free(DynamicArray* array) {
    // Check if the data array is not NULL
    if (array == NULL) return;

    free(array->data);
    array->data         = NULL;
    array->size         = 0;
    array->capacity     = 0;
    array->element_size = 0;
}
