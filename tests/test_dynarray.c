/* ./cstruct/tests/test_dynarray.c */

#include "test.h"
#include "dynarray.h"

int main(void) {
    DynArray array;

    // Invalid initialization arguments
    CHECK(dynarray_init(NULL, 10, sizeof(int)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dynarray_init(&array, 0, sizeof(int)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dynarray_init(&array, 10, 0) == CSTRUCT_ERR_INVALID_ARG);

    // Valid initialization
    CHECK(dynarray_init(&array, 2, sizeof(int)) == CSTRUCT_SUCCESS);
    CHECK(array.size == 0);
    CHECK(array.capacity == 2);
    CHECK(array.element_size == sizeof(int));

    // Insert two elements
    int a = 42, b = 43;
    CHECK(dynarray_insert(&array, &a) == CSTRUCT_SUCCESS);
    CHECK(dynarray_insert(&array, &b) == CSTRUCT_SUCCESS);
    CHECK(array.size == 2);
    CHECK(*(int *)dynarray_get(&array, 0) == 42);
    CHECK(*(int *)dynarray_get(&array, 1) == 43);

    // Insert a third which forces a resize from 2 to 4 elements
    int c = 44;
    CHECK(dynarray_insert(&array, &c) == CSTRUCT_SUCCESS);
    CHECK(array.size == 3);
    CHECK(array.capacity == 4);
    CHECK(*(int *)dynarray_get(&array, 2) == 44);

    // Get out of bounds
    CHECK(dynarray_get(&array, 3) == NULL);
    CHECK(dynarray_get(&array, 100) == NULL);
    CHECK(dynarray_get(NULL, 0) == NULL);

    // Delete middle elements
    CHECK(dynarray_delete(&array, 1) == CSTRUCT_SUCCESS);
    CHECK(array.size == 2);
    CHECK(*(int *)dynarray_get(&array, 0) == 42);
    CHECK(*(int *)dynarray_get(&array, 1) == 44);

    // Delete out of bounds
    CHECK(dynarray_delete(&array, 5) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dynarray_delete(NULL, 0) == CSTRUCT_ERR_INVALID_ARG);

    // Insert NULL value
    CHECK(dynarray_insert(&array, NULL) == CSTRUCT_ERR_INVALID_ARG);

    // Free and verify clean state
    dynarray_free(&array);
    CHECK(array.data == NULL);
    CHECK(array.size == 0);
    CHECK(array.capacity == 0);
    CHECK(array.element_size == 0);

    // free(NULL) is safe by C standards
    dynarray_free(NULL);

    REPORT();
}
