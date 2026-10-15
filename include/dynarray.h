/* ./cstruct/include/dynarray.h */

#ifndef CSTRUCT_DYNARRAY_H
#define CSTRUCT_DYNARRAY_H

#include <stddef.h>

#include "error.h"

typedef struct DynArray {
   void*    data;           // Pointer to the data array
   size_t   size;           // Current size of the array
   size_t   capacity;       // Maximum capacity of the array
   size_t   element_size;   // Size of a single data element in bytes
} DynArray;

ErrorCode dynarray_init(DynArray* array, size_t capacity, size_t element_size);
ErrorCode dynarray_resize(DynArray* array, size_t new_capacity);
ErrorCode dynarray_insert(DynArray* array, const void* new_data);
ErrorCode dynarray_delete(DynArray* array, size_t index);

void* dynarray_get(const DynArray* array, size_t index);
void dynarray_free(DynArray* array);

#endif /* CSTRUCT_DYNARRAY_H */
