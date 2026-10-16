/* ./cstruct/tests/test_dlist.c */

#include "test.h"
#include "dlist.h"

typedef struct {
    void**  items;
    size_t  count;
} Collector;

static void collect(void* data, void* context) {
    Collector* c            = context;
    c->items[c->count++]    = data;
}

int main(void) {
    DList dlist;

    // Invalid init
    CHECK(dlist_init(NULL) == CSTRUCT_ERR_INVALID_ARG);

    // Valid init
    CHECK(dlist_init(&dlist) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 0);
    CHECK(dlist.head == NULL);
    CHECK(dlist.tail == NULL);
    CHECK(dlist_get_head(&dlist) == NULL);
    CHECK(dlist_get_tail(&dlist) == NULL);

    // Empty list
    CHECK(dlist_delete_front(&dlist) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_back(&dlist) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_at(&dlist, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_search_by_position(&dlist, 0) == NULL);

    // Insert at back
    int a = 1, b = 2, c = 3;
    CHECK(dlist_insert_back(&dlist, &a) == CSTRUCT_SUCCESS);
    CHECK(dlist_insert_back(&dlist, &b) == CSTRUCT_SUCCESS);
    CHECK(dlist_insert_back(&dlist, &c) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 3);
    CHECK(dlist_get_head(&dlist) == &a);
    CHECK(dlist_get_tail(&dlist) == &c);

    // Search by position
    CHECK(dlist_search_by_position(&dlist, 0) == &a);
    CHECK(dlist_search_by_position(&dlist, 1) == &b);
    CHECK(dlist_search_by_position(&dlist, 2) == &c);
    CHECK(dlist_search_by_position(&dlist, 3) == NULL);

    // Search by pointer
    CHECK(dlist_search_by_pointer(&dlist, &b) == &b);
    int other = 99;
    CHECK(dlist_search_by_pointer(&dlist, &other) == NULL);

    // Forward and backward traversal agree
    void* fwd[8];
    void* bwd[8];
    Collector cf = { .items = fwd, .count = 0 };
    Collector cb = { .items = bwd, .count = 0 };
    dlist_forward(&dlist, collect, &cf);
    dlist_backward(&dlist, collect, &cb);
    CHECK(cf.count == 3);
    CHECK(cb.count == 3);
    CHECK(fwd[0] == &a); CHECK(fwd[1] == &b); CHECK(fwd[2] == &c);
    CHECK(bwd[0] == &c); CHECK(bwd[1] == &b); CHECK(bwd[2] == &a);

    // Insert at front
    int front = 0;
    CHECK(dlist_insert_front(&dlist, &front) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 4);
    CHECK(dlist_get_head(&dlist) == &front);
    CHECK(dlist_get_tail(&dlist) == &c);

    // Bidirectional check after front insert
    cf.count = 0;
    cb.count = 0;
    dlist_forward(&dlist, collect, &cf);
    dlist_backward(&dlist, collect, &cb);
    CHECK(cf.count == 4 && cb.count == 4);
    CHECK(fwd[0] == &front); CHECK(fwd[1] == &a); CHECK(fwd[2] == &b); CHECK(fwd[3] == &c);
    CHECK(bwd[0] == &c); CHECK(bwd[1] == &b); CHECK(bwd[2] == &a); CHECK(bwd[3] == &front);

    // Insert at position (middle)
    int mid = 42;
    CHECK(dlist_insert_at(&dlist, &mid, 2) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 5);
    CHECK(dlist_search_by_position(&dlist, 0) == &front);
    CHECK(dlist_search_by_position(&dlist, 1) == &a);
    CHECK(dlist_search_by_position(&dlist, 2) == &mid);
    CHECK(dlist_search_by_position(&dlist, 3) == &b);
    CHECK(dlist_search_by_position(&dlist, 4) == &c);

    cf.count = 0;
    cb.count = 0;
    dlist_forward(&dlist, collect, &cf);
    dlist_backward(&dlist, collect, &cb);
    CHECK(cf.count == 5 && cb.count == 5);
    CHECK(fwd[0] == &front); CHECK(fwd[1] == &a); CHECK(fwd[2] == &mid);
    CHECK(fwd[3] == &b); CHECK(fwd[4] == &c);
    CHECK(bwd[0] == &c); CHECK(bwd[1] == &b); CHECK(bwd[2] == &mid);
    CHECK(bwd[3] == &a); CHECK(bwd[4] == &front);

    // Insert at position 0
    int front2 = -1;
    CHECK(dlist_insert_at(&dlist, &front2, 0) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 6);
    CHECK(dlist_get_head(&dlist) == &front2);
    CHECK(dlist_search_by_position(&dlist, 1) == &front);

    // Insert at index == size
    int last = 100;
    CHECK(dlist_insert_at(&dlist, &last, dlist.size) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 7);
    CHECK(dlist_get_tail(&dlist) == &last);
    CHECK(dlist_search_by_position(&dlist, 6) == &last);

    // Delete at position (middle) removes the right node
    CHECK(dlist_delete_at(&dlist, 3) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 6);
    CHECK(dlist_search_by_position(&dlist, 0) == &front2);
    CHECK(dlist_search_by_position(&dlist, 1) == &front);
    CHECK(dlist_search_by_position(&dlist, 2) == &a);
    CHECK(dlist_search_by_position(&dlist, 3) == &b);
    CHECK(dlist_search_by_position(&dlist, 4) == &c);
    CHECK(dlist_search_by_position(&dlist, 5) == &last);
    CHECK(dlist_search_by_pointer(&dlist, &mid) == NULL);

    // Bidirectional check after middle delete
    cf.count = 0;
    cb.count = 0;
    dlist_forward(&dlist, collect, &cf);
    dlist_backward(&dlist, collect, &cb);
    CHECK(cf.count == 6 && cb.count == 6);
    CHECK(fwd[0] == &front2); CHECK(fwd[1] == &front); CHECK(fwd[2] == &a);
    CHECK(fwd[3] == &b); CHECK(fwd[4] == &c); CHECK(fwd[5] == &last);
    CHECK(bwd[0] == &last); CHECK(bwd[1] == &c); CHECK(bwd[2] == &b);
    CHECK(bwd[3] == &a); CHECK(bwd[4] == &front); CHECK(bwd[5] == &front2);

    // Delete at position 0
    CHECK(dlist_delete_at(&dlist, 0) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 5);
    CHECK(dlist_get_head(&dlist) == &front);
    CHECK(dlist_get_tail(&dlist) == &last);

    // Delete at last index
    CHECK(dlist_delete_at(&dlist, dlist.size - 1) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 4);
    CHECK(dlist_get_tail(&dlist) == &c);

    // Delete front
    CHECK(dlist_delete_front(&dlist) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 3);
    CHECK(dlist_get_head(&dlist) == &a);
    CHECK(dlist_search_by_position(&dlist, 0) == &a);
    CHECK(dlist_search_by_position(&dlist, 1) == &b);
    CHECK(dlist_search_by_position(&dlist, 2) == &c);

    // Delete back
    CHECK(dlist_delete_back(&dlist) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 2);
    CHECK(dlist_get_tail(&dlist) == &b);

    // Transition 2 -> 1 -> 0 verifying head/tail each step
    CHECK(dlist_delete_back(&dlist) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 1);
    CHECK(dlist.head == dlist.tail);
    CHECK(dlist_get_head(&dlist) == &a);
    CHECK(dlist_get_tail(&dlist) == &a);

    CHECK(dlist_delete_back(&dlist) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 0);
    CHECK(dlist.head == NULL);
    CHECK(dlist.tail == NULL);

    // Underflow on empty after drain
    CHECK(dlist_delete_front(&dlist) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_back(&dlist)  == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_at(&dlist, 0) == CSTRUCT_ERR_INVALID_ARG);

    // Reuse after empty
    int only = 7;
    CHECK(dlist_insert_front(&dlist, &only) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 1);
    CHECK(dlist.head == dlist.tail);
    CHECK(dlist_get_head(&dlist) == &only);
    CHECK(dlist_get_tail(&dlist) == &only);
    CHECK(dlist_delete_front(&dlist) == CSTRUCT_SUCCESS);
    CHECK(dlist.size == 0);
    CHECK(dlist.head == NULL);
    CHECK(dlist.tail == NULL);

    // Invalid arguments
    CHECK(dlist_insert_front(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_insert_front(&dlist, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_insert_back(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_insert_back(&dlist, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_insert_at(NULL, &a, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_insert_at(&dlist, NULL, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_insert_at(&dlist, &a, 99) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_front(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_back(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_delete_at(NULL, 0) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(dlist_search_by_position(NULL, 0) == NULL);
    CHECK(dlist_search_by_pointer(NULL, &a) == NULL);
    CHECK(dlist_search_by_pointer(&dlist, NULL) == NULL);
    CHECK(dlist_get_head(NULL) == NULL);
    CHECK(dlist_get_tail(NULL) == NULL);

    // Traverse with NULL args is a safe
    Collector col = { .items = fwd, .count = 0 };
    dlist_forward(NULL, collect, &col);
    dlist_forward(&dlist, NULL, &col);
    dlist_backward(NULL, collect, &col);
    dlist_backward(&dlist, NULL, &col);

    // Free resets state
    dlist_free(&dlist);
    CHECK(dlist.head == NULL);
    CHECK(dlist.tail == NULL);
    CHECK(dlist.size == 0);
    dlist_free(NULL);

    REPORT();
}
