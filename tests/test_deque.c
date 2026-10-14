/* ./cstruct/tests/test_deque.c */

#include "test.h"
#include "deque.h"

int main(void) {
    Deque deque;

    /* Invalid init */
    CHECK(deque_init(NULL) == CSTRUCT_ERR_INVALID_ARG);

    /* Valid init */
    CHECK(deque_init(&deque) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 0);
    CHECK(deque.data.head == NULL);
    CHECK(deque.data.tail == NULL);

    /* Empty deque: peek NULL, delete underflows */
    CHECK(deque_peek_front(&deque) == NULL);
    CHECK(deque_peek_back(&deque) == NULL);
    CHECK(deque_delete_front(&deque) == CSTRUCT_ERR_UNDERFLOW);
    CHECK(deque_delete_back(&deque) == CSTRUCT_ERR_UNDERFLOW);

    /* Insert at back: [a, b, c] */
    int a = 1, b = 2, c = 3;
    CHECK(deque_insert_back(&deque, &a) == CSTRUCT_SUCCESS);
    CHECK(deque_insert_back(&deque, &b) == CSTRUCT_SUCCESS);
    CHECK(deque_insert_back(&deque, &c) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 3);
    CHECK(*(int*)deque_peek_front(&deque) == 1);    // a
    CHECK(*(int*)deque_peek_back(&deque) == 3);     // c

    /* Insert at front */
    int front = 0;
    CHECK(deque_insert_front(&deque, &front) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 4);
    CHECK(*(int*)deque_peek_front(&deque) == 0);    // front
    CHECK(*(int*)deque_peek_back(&deque) == 3);    // c, unchanged

    /* Deque is now: [front, a, b, c] */

    /* Delete from front */
    CHECK(deque_delete_front(&deque) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 3);
    CHECK(*(int*)deque_peek_front(&deque) == 1);    // a

    /* Delete from back */
    CHECK(deque_delete_back(&deque) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 2);
    CHECK(*(int*)deque_peek_back(&deque) == 2);     // b

    /* Deque is now: [a, b] */

    /* Interleaved: front and back stay distinct */
    int x = 10, y = 20;
    CHECK(deque_insert_front(&deque, &x) == CSTRUCT_SUCCESS);
    CHECK(deque_insert_back(&deque, &y) == CSTRUCT_SUCCESS);

    /* [x, a, b, y] */
    CHECK(*(int*)deque_peek_front(&deque) == 10);
    CHECK(*(int*)deque_peek_back(&deque) == 20);

    CHECK(deque_delete_front(&deque) == CSTRUCT_SUCCESS);   // remove x
    CHECK(deque_delete_back(&deque) == CSTRUCT_SUCCESS);    // remove y

    /* back to [a, b] */
    CHECK(deque.data.size == 2);
    CHECK(*(int*)deque_peek_front(&deque) == 1);
    CHECK(*(int*)deque_peek_back(&deque) == 2);

    /* Drain to empty from both ends */
    CHECK(deque_delete_front(&deque) == CSTRUCT_SUCCESS);   // remove a
    CHECK(deque_delete_back(&deque) == CSTRUCT_SUCCESS);    // remove b
    CHECK(deque.data.size == 0);
    CHECK(deque.data.head == NULL);
    CHECK(deque.data.tail == NULL);

    /* Underflow again */
    CHECK(deque_delete_front(&deque) == CSTRUCT_ERR_UNDERFLOW);
    CHECK(deque_delete_back(&deque) == CSTRUCT_ERR_UNDERFLOW);

    /* Single element: delete from either end works */
    int only = 99;
    CHECK(deque_insert_back(&deque, &only) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 1);
    CHECK(*(int*)deque_peek_front(&deque) == 99);
    CHECK(*(int*)deque_peek_back(&deque) == 99);
    CHECK(deque_delete_front(&deque) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 0);

    CHECK(deque_insert_front(&deque, &only) == CSTRUCT_SUCCESS);
    CHECK(deque_delete_back(&deque) == CSTRUCT_SUCCESS);
    CHECK(deque.data.size == 0);

    /* Invalid arguments */
    CHECK(deque_insert_front(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(deque_insert_front(&deque, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(deque_insert_back(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(deque_insert_back(&deque, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(deque_delete_front(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(deque_delete_back(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(deque_peek_front(NULL) == NULL);
    CHECK(deque_peek_back(NULL) == NULL);

    /* Free resets state; free(NULL) is a no-op */
    deque_free(&deque);
    CHECK(deque.data.head == NULL);
    CHECK(deque.data.tail == NULL);
    CHECK(deque.data.size == 0);
    deque_free(NULL);

    REPORT();
}
