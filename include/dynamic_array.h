/* ./cstruct/include/dynamic_array.h */

#ifndef CSTRUCT_DYNAMIC_ARRAY_H
#define CSTRUCT_DYNAMIC_ARRAY_H

#include <stddef.h>

#include "error.h"

typedef struct DynamicArray {
   void*    data;           // Pointer to the data array
   size_t   size;           // Current size of the array
   size_t   capacity;       // Maximum capacity of the array
   size_t   element_size;   // Size of a single data element in bytes
} DynamicArray;

ErrorCode dynamic_array_init(DynamicArray* array, size_t capacity, size_t element_size);
ErrorCode dynamic_array_resize(DynamicArray* array, size_t new_capacity);
ErrorCode dynamic_array_insert(DynamicArray* array, const void* new_data);
ErrorCode dynamic_array_delete(DynamicArray* array, size_t index);

void* dynamic_array_get(const DynamicArray* array, size_t index);
void dynamic_array_free(DynamicArray* array);

#endif /* CSTRUCT_DYNAMIC_ARRAY_H */
