/* ./cstruct/tests/test_list.c */

#include "test.h"
#include "list.h"

typedef struct {
    void**  items;
    size_t  count;
} Collector;

static void count_element(void* data, void* context) {
    (void)data;
    (*(size_t*)context)++;
}

static void collect(void* data, void* context) {
    Collector* c            = context;
    c->items[c->count++]    = data;
}

int main(void) {
    List list;

    // Invalid init
    CHECK(list_init(NULL) == CSTRUCT_ERR_INVALID_ARG);

    // Valid init
    CHECK(list_init(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 0);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(list_get_head(&list) == NULL);
    CHECK(list_get_tail(&list) == NULL);

    // Insert at end
    int a = 1, b = 2, c = 3;
    CHECK(list_insert_back(&list, &a) == CSTRUCT_SUCCESS);
    CHECK(list_insert_back(&list, &b) == CSTRUCT_SUCCESS);
    CHECK(list_insert_back(&list, &c) == CSTRUCT_SUCCESS);
    CHECK(list.size == 3);
    CHECK(list_get_head(&list) == &a);
    CHECK(list_get_tail(&list) == &c);

    // Search by position
    CHECK(list_search_by_position(&list, 0) == &a);
    CHECK(list_search_by_position(&list, 1) == &b);
    CHECK(list_search_by_position(&list, 2) == &c);
    // Out of bounds
    CHECK(list_search_by_position(&list, 3) == NULL);

    // Search by pointer
    CHECK(list_search_by_pointer(&list, &b) == &b);
    int other = 99;
    CHECK(list_search_by_pointer(&list, &other) == NULL);

    // Traverse (count)
    size_t n = 0;
    list_traverse(&list, count_element, &n);
    CHECK(n == 3);

    // Traverse (collect and verify order)
    void* items[8];
    Collector col = { .items = items, .count = 0 };
    list_traverse(&list, collect, &col);
    CHECK(col.count == 3);
    CHECK(col.items[0] == &a);
    CHECK(col.items[1] == &b);
    CHECK(col.items[2] == &c);

    // Insert at beginning
    int front = 0;
    CHECK(list_insert_front(&list, &front) == CSTRUCT_SUCCESS);
    CHECK(list.size == 4);
    CHECK(list_search_by_position(&list, 0) == &front);
    CHECK(list_search_by_position(&list, 1) == &a);
    CHECK(list_search_by_position(&list, 3) == &c);
    // Tail unchanged
    CHECK(list.tail != NULL);
    CHECK(list_get_head(&list) == &front);
    CHECK(list_get_tail(&list) == &c);

    // Insert at position (middle)
    int mid = 42;
    CHECK(list_insert_at(&list, &mid, 2) == CSTRUCT_SUCCESS);
    CHECK(list.size == 5);
    CHECK(list_search_by_position(&list, 0) == &front);
    CHECK(list_search_by_position(&list, 1) == &a);
    CHECK(list_search_by_position(&list, 2) == &mid);
    CHECK(list_search_by_position(&list, 3) == &b);
    CHECK(list_search_by_position(&list, 4) == &c);

    // Insert at position (index == size, i.e. append)
    int last = 100;
    CHECK(list_insert_at(&list, &last, list.size) == CSTRUCT_SUCCESS);
    CHECK(list.size == 6);
    CHECK(list_search_by_position(&list, 5) == &last);
    CHECK(list_get_tail(&list) == &last);

    // Delete at beginning
    CHECK(list_delete_front(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 5);
    CHECK(list_search_by_position(&list, 0) == &a);
    CHECK(list_search_by_position(&list, 1) == &mid);

    // Delete at end
    CHECK(list_delete_back(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 4);
    CHECK(list_search_by_position(&list, 3) == &c);
    CHECK(list_search_by_position(&list, 4) == NULL);
    CHECK(list_get_tail(&list) == &c);

    // Delete at position (middle)
    // list is now: [a, mid, b, c]
    CHECK(list_delete_at(&list, 1) == CSTRUCT_SUCCESS);
    CHECK(list.size == 3);
    CHECK(list_search_by_position(&list, 0) == &a);
    CHECK(list_search_by_position(&list, 1) == &b);
    CHECK(list_search_by_position(&list, 2) == &c);

    // Delete at position (index 0, delegates to beginning)
    CHECK(list_delete_at(&list, 0) == CSTRUCT_SUCCESS);
    CHECK(list.size == 2);
    CHECK(list_search_by_position(&list, 0) == &b);

    // Delete at position (last index, delegates to end)
    // list is now = [b, c]
    CHECK(list_delete_at(&list, 1) == CSTRUCT_SUCCESS);
    CHECK(list.size == 1);
    CHECK(list_search_by_position(&list, 0) == &b);
    CHECK(list_get_tail(&list) == &b);

    // Invalid arguments
    CHECK(list_insert_back(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_insert_back(&list, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_insert_front(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_insert_front(&list, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_insert_at(NULL, &a, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_insert_at(&list, NULL, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_insert_at(&list, &a, 99) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_delete_at(&list, 99) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_delete_at(NULL, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_delete_front(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_delete_back(NULL) == CSTRUCT_ERR_INVALID_ARG);

    // Search on NULL/invalid
    CHECK(list_search_by_position(NULL, 0) == NULL);
    CHECK(list_search_by_pointer(NULL, &a) == NULL);
    CHECK(list_search_by_pointer(&list, NULL) == NULL);
    CHECK(list_get_head(NULL) == NULL);
    CHECK(list_get_tail(NULL) == NULL);

    // Traverse with NULL args is safe
    list_traverse(NULL, collect, &col);
    list_traverse(&list, NULL, &col);

    // Drain to empty
    // list is now: [b]
    CHECK(list_delete_front(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 0);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(list_delete_front(&list) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(list_delete_back(&list) == CSTRUCT_ERR_INVALID_ARG);

    // Free resets state
    list_free(&list);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(list.size == 0);
    list_free(NULL);

    REPORT();
}
