/* ./cstruct/include/stack.h */

#ifndef CSTRUCT_STACK_H
#define CSTRUCT_STACK_H

#include <stddef.h>

#include "error.h"
#include "dynarray.h"

typedef struct Stack {
    DynArray data; // Dynamic array to hold the stack elements
} Stack;

ErrorCode stack_init(Stack* stack, size_t capacity, size_t element_size);
ErrorCode stack_push(Stack* stack, const void* value);
ErrorCode stack_pop(Stack* stack);

void* stack_peek(const Stack* stack);
void stack_free(Stack* stack);

#endif /* CSTRUCT_STACK_H */
