/* ./cstruct/tests/test_stack.c */

#include "test.h"
#include "stack.h"

int main(void) {
    Stack stack;

    // Invalid init
    CHECK(stack_init(NULL, 4, sizeof(int)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(stack_init(&stack, 0, sizeof(int)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(stack_init(&stack, 4, 0) == CSTRUCT_ERR_INVALID_ARG);

    // Valid init
    CHECK(stack_init(&stack, 4, sizeof(int)) == CSTRUCT_SUCCESS);
    CHECK(stack.data.size == 0);
    CHECK(stack.data.capacity == 4);
    CHECK(stack.data.element_size == sizeof(int));

    // Empty stack (peek is NULL and pop underflows)
    CHECK(stack_peek(&stack) == NULL);
    CHECK(stack_pop(&stack) == CSTRUCT_ERR_UNDERFLOW);

    // Push three elements
    int a = 1, b = 2, c = 3;
    CHECK(stack_push(&stack, &a) == CSTRUCT_SUCCESS);
    CHECK(stack_push(&stack, &b) == CSTRUCT_SUCCESS);
    CHECK(stack_push(&stack, &c) == CSTRUCT_SUCCESS);
    CHECK(stack.data.size == 3);

    // Peek returns the top without modifying the stack
    int* top = (int*)stack_peek(&stack);
    CHECK(top != NULL);
    CHECK(*top == 3);
    // Peek did not remove anything
    CHECK(stack.data.size == 3);

    // Pop in LIFO order
    CHECK(stack_pop(&stack) == CSTRUCT_SUCCESS);
    CHECK(stack.data.size == 2);
    top = (int*)stack_peek(&stack);
    CHECK(top != NULL && *top == 2);

    CHECK(stack_pop(&stack) == CSTRUCT_SUCCESS);
    top = (int*)stack_peek(&stack);
    CHECK(top != NULL && *top == 1);

    CHECK(stack_pop(&stack) == CSTRUCT_SUCCESS);
    CHECK(stack.data.size == 0);

    // Empty again
    CHECK(stack_peek(&stack) == NULL);
    CHECK(stack_pop(&stack) == CSTRUCT_ERR_UNDERFLOW);

    // Invalid arguments
    CHECK(stack_push(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(stack_push(&stack, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(stack_pop(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(stack_peek(NULL) == NULL);

    // Push more than the initial capacity to force growth
    int vals[5] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        CHECK(stack_push(&stack, &vals[i]) == CSTRUCT_SUCCESS);
    }

    CHECK(stack.data.size == 5);
    // Grew past the initial 4
    CHECK(stack.data.capacity >= 5);
    top = (int*)stack_peek(&stack);
    CHECK(top != NULL && *top == 50);

    // Drain (verify every element came out in reverse)
    for (int i = 4; i >= 0; i--) {
        top = (int*)stack_peek(&stack);
        CHECK(top != NULL && *top == vals[i]);
        CHECK(stack_pop(&stack) == CSTRUCT_SUCCESS);
    }

    CHECK(stack.data.size == 0);

    // Free resets inner state
    stack_free(&stack);
    CHECK(stack.data.data == NULL);
    CHECK(stack.data.size == 0);
    CHECK(stack.data.capacity == 0);
    CHECK(stack.data.element_size == 0);
    stack_free(NULL);

    REPORT();
}
