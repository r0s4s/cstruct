/* ./cstruct/tests/test_dynamic_array.c */

#include "test.h"

#include "../include/dynamic_array.h"

int main(void) {
    DynamicArray array;

    /* Invalid initialization arguments */
    CHECK(dynamic_array_init(NULL, 10, sizeof(int)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dynamic_array_init(&array, 0, sizeof(int)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dynamic_array_init(&array, 10, 0) == CSTRUCT_ERR_INVALID_ARG);

    /* Valid initialization */
    CHECK(dynamic_array_init(&array, 2, sizeof(int)) == CSTRUCT_SUCCESS);
    CHECK(array.size == 0);
    CHECK(array.capacity == 2);
    CHECK(array.element_size == sizeof(int));

    /* Insert two elements */
    int a = 42, b = 43;
    CHECK(dynamic_array_insert(&array, &a) == CSTRUCT_SUCCESS);
    CHECK(dynamic_array_insert(&array, &b) == CSTRUCT_SUCCESS);
    CHECK(array.size == 2);
    CHECK(*(int *)dynamic_array_get(&array, 0) == 42);
    CHECK(*(int *)dynamic_array_get(&array, 1) == 43);

    /* Insert a third which forces a resize (2 -> 4) */
    int c = 44;
    CHECK(dynamic_array_insert(&array, &c) == CSTRUCT_SUCCESS);
    CHECK(array.size == 3);
    CHECK(array.capacity == 4);
    CHECK(*(int *)dynamic_array_get(&array, 2) == 44);

    /* Get out of bounds */
    CHECK(dynamic_array_get(&array, 3) == NULL);
    CHECK(dynamic_array_get(&array, 100) == NULL);
    CHECK(dynamic_array_get(NULL, 0) == NULL);

    /* Delete middle */
    CHECK(dynamic_array_delete(&array, 1) == CSTRUCT_SUCCESS);
    CHECK(array.size == 2);
    CHECK(*(int *)dynamic_array_get(&array, 0) == 42);
    CHECK(*(int *)dynamic_array_get(&array, 1) == 44);

    /* Delete out of bounds */
    CHECK(dynamic_array_delete(&array, 5) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dynamic_array_delete(NULL, 0) == CSTRUCT_ERR_INVALID_ARG);

    /* Insert NULL */
    CHECK(dynamic_array_insert(&array, NULL) == CSTRUCT_ERR_INVALID_ARG);

    /* Free and verify clean state */
    dynamic_array_free(&array);
    CHECK(array.data == NULL);
    CHECK(array.size == 0);
    CHECK(array.capacity == 0);
    CHECK(array.element_size == 0);

    /* Free(NULL) is a no-op */
    dynamic_array_free(NULL);

    REPORT();
}
