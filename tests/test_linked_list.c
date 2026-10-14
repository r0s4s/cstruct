/* ./cstruct/tests/test_linked_list.c */

#include "test.h"
#include "linked_list.h"

typedef struct {
    void** items;
    size_t count;
} Collector;

static void count_element(void* data, void* context) {
    (void)data;
    (*(size_t*)context)++;
}

static void collect(void* data, void* context) {
    Collector* c = context;
    c->items[c->count++] = data;
}

int main(void) {
    LinkedList list;

    /* Invalid init */
    CHECK(linked_list_init(NULL) == CSTRUCT_ERR_INVALID_ARG);

    /* Valid init */
    CHECK(linked_list_init(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 0);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(linked_list_get_head(&list) == NULL);
    CHECK(linked_list_get_tail(&list) == NULL);

    /* Insert at end */
    int a = 1, b = 2, c = 3;
    CHECK(linked_list_insert_at_end(&list, &a) == CSTRUCT_SUCCESS);
    CHECK(linked_list_insert_at_end(&list, &b) == CSTRUCT_SUCCESS);
    CHECK(linked_list_insert_at_end(&list, &c) == CSTRUCT_SUCCESS);
    CHECK(list.size == 3);
    CHECK(linked_list_get_head(&list) == &a);
    CHECK(linked_list_get_tail(&list) == &c);

    /* Search by position */
    CHECK(linked_list_search_by_position(&list, 0) == &a);
    CHECK(linked_list_search_by_position(&list, 1) == &b);
    CHECK(linked_list_search_by_position(&list, 2) == &c);
    CHECK(linked_list_search_by_position(&list, 3) == NULL); // out of bounds

    /* Search by pointer */
    CHECK(linked_list_search_by_pointer(&list, &b) == &b);
    int other = 99;
    CHECK(linked_list_search_by_pointer(&list, &other) == NULL);

    /* Traverse (count) */
    size_t n = 0;
    linked_list_traverse(&list, count_element, &n);
    CHECK(n == 3);

    /* Traverse (collect and verify order) */
    void* items[8];
    Collector col = { .items = items, .count = 0 };
    linked_list_traverse(&list, collect, &col);
    CHECK(col.count == 3);
    CHECK(col.items[0] == &a);
    CHECK(col.items[1] == &b);
    CHECK(col.items[2] == &c);

    /* Insert at beginning */
    int front = 0;
    CHECK(linked_list_insert_at_beginning(&list, &front) == CSTRUCT_SUCCESS);
    CHECK(list.size == 4);
    CHECK(linked_list_search_by_position(&list, 0) == &front);
    CHECK(linked_list_search_by_position(&list, 1) == &a);
    CHECK(linked_list_search_by_position(&list, 3) == &c);
    CHECK(list.tail != NULL); // tail unchanged
    CHECK(linked_list_get_head(&list) == &front);
    CHECK(linked_list_get_tail(&list) == &c);

    /* Insert at position (middle) */
    int mid = 42;
    CHECK(linked_list_insert_at_position(&list, &mid, 2) == CSTRUCT_SUCCESS);
    CHECK(list.size == 5);
    CHECK(linked_list_search_by_position(&list, 0) == &front);
    CHECK(linked_list_search_by_position(&list, 1) == &a);
    CHECK(linked_list_search_by_position(&list, 2) == &mid);
    CHECK(linked_list_search_by_position(&list, 3) == &b);
    CHECK(linked_list_search_by_position(&list, 4) == &c);

    /* Insert at position (index == size, i.e. append) */
    int last = 100;
    CHECK(linked_list_insert_at_position(&list, &last, list.size) == CSTRUCT_SUCCESS);
    CHECK(list.size == 6);
    CHECK(linked_list_search_by_position(&list, 5) == &last);
    CHECK(linked_list_get_tail(&list) == &last);

    /* Delete at beginning */
    CHECK(linked_list_delete_at_beginning(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 5);
    CHECK(linked_list_search_by_position(&list, 0) == &a);
    CHECK(linked_list_search_by_position(&list, 1) == &mid);

    /* Delete at end */
    CHECK(linked_list_delete_at_end(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 4);
    CHECK(linked_list_search_by_position(&list, 3) == &c);
    CHECK(linked_list_search_by_position(&list, 4) == NULL);  // gone
    CHECK(linked_list_get_tail(&list) == &c);

    /* Delete at position (middle) */

    // list is now: [a, mid, b, c]
    CHECK(linked_list_delete_at_position(&list, 1) == CSTRUCT_SUCCESS);
    CHECK(list.size == 3);
    CHECK(linked_list_search_by_position(&list, 0) == &a);
    CHECK(linked_list_search_by_position(&list, 1) == &b);
    CHECK(linked_list_search_by_position(&list, 2) == &c);

    /* Delete at position (index 0, delegates to beginning) */
    CHECK(linked_list_delete_at_position(&list, 0) == CSTRUCT_SUCCESS);
    CHECK(list.size == 2);
    CHECK(linked_list_search_by_position(&list, 0) == &b);

    /* Delete at position (last index, delegates to end) */

    // list is now = [b, c]
    CHECK(linked_list_delete_at_position(&list, 1) == CSTRUCT_SUCCESS);
    CHECK(list.size == 1);
    CHECK(linked_list_search_by_position(&list, 0) == &b);
    CHECK(linked_list_get_tail(&list) == &b);

    /* Invalid arguments */
    CHECK(linked_list_insert_at_end(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_insert_at_end(&list, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_insert_at_beginning(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_insert_at_beginning(&list, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_insert_at_position(NULL, &a, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_insert_at_position(&list, NULL, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_insert_at_position(&list, &a, 99) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_delete_at_position(&list, 99) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_delete_at_position(NULL, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_delete_at_beginning(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_delete_at_end(NULL) == CSTRUCT_ERR_INVALID_ARG);

    /* Search on NULL/invalid */
    CHECK(linked_list_search_by_position(NULL, 0) == NULL);
    CHECK(linked_list_search_by_pointer(NULL, &a) == NULL);
    CHECK(linked_list_search_by_pointer(&list, NULL) == NULL);
    CHECK(linked_list_get_head(NULL) == NULL);
    CHECK(linked_list_get_tail(NULL) == NULL);

    /* Traverse with NULL args is a safe no-op */
    linked_list_traverse(NULL, collect, &col);   // must not crash
    linked_list_traverse(&list, NULL, &col);     // must not crash

    /* Drain to empty */

    // list is now: [b]
    CHECK(linked_list_delete_at_beginning(&list) == CSTRUCT_SUCCESS);
    CHECK(list.size == 0);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(linked_list_delete_at_beginning(&list) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(linked_list_delete_at_end(&list) == CSTRUCT_ERR_INVALID_ARG);

    /* Free resets state, free(NULL) is a no-op */
    linked_list_free(&list);
    CHECK(list.head == NULL);
    CHECK(list.tail == NULL);
    CHECK(list.size == 0);
    linked_list_free(NULL);

    REPORT();
}
