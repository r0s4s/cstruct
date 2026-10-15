/* ./cstruct/tests/test_queue.c */

#include "test.h"
#include "queue.h"

int main(void) {
    Queue queue;

    // Invalid init
    CHECK(queue_init(NULL) == CSTRUCT_ERR_INVALID_ARG);

    // Valid init
    CHECK(queue_init(&queue) == CSTRUCT_SUCCESS);
    CHECK(queue.data.size == 0);
    CHECK(queue.data.head == NULL);
    CHECK(queue.data.tail == NULL);

    // Empty queue
    CHECK(queue_peek(&queue) == NULL);
    CHECK(queue_dequeue(&queue) == CSTRUCT_ERR_UNDERFLOW);

    // Enqueue three elements
    int a = 1, b = 2, c = 3;
    CHECK(queue_enqueue(&queue, &a) == CSTRUCT_SUCCESS);
    CHECK(queue_enqueue(&queue, &b) == CSTRUCT_SUCCESS);
    CHECK(queue_enqueue(&queue, &c) == CSTRUCT_SUCCESS);
    CHECK(queue.data.size == 3);

    // Peek returns the front not the back
    int* front = (int*)queue_peek(&queue);
    CHECK(front != NULL);
    // a, the first enqueued
    CHECK(*front == 1);
    // peek did not remove anything
    CHECK(queue.data.size == 3);

    // Dequeue in FIFO order
    CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    CHECK(queue.data.size == 2);
    front = (int*)queue_peek(&queue);
    CHECK(front != NULL && *front == 2);

    CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    front = (int*)queue_peek(&queue);
    CHECK(front != NULL && *front == 3);

    CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    CHECK(queue.data.size == 0);

    // Empty again
    CHECK(queue_peek(&queue) == NULL);
    CHECK(queue_dequeue(&queue) == CSTRUCT_ERR_UNDERFLOW);

    // Invalid arguments
    CHECK(queue_enqueue(NULL, &a) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(queue_enqueue(&queue, NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(queue_dequeue(NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(queue_peek(NULL) == NULL);

    // Enqueue again after draining
    int x = 100, y = 200;
    CHECK(queue_enqueue(&queue, &x) == CSTRUCT_SUCCESS);
    CHECK(queue_enqueue(&queue, &y) == CSTRUCT_SUCCESS);
    CHECK(queue.data.size == 2);
    front = (int*)queue_peek(&queue);
    CHECK(front != NULL && *front == 100);

    // Full FIFO drain with mixed values
    int vals[5] = {10, 20, 30, 40, 50};

    // empty first so the drain is clean
    while (queue.data.size > 0) {
        CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    }

    for (int i = 0; i < 5; i++) {
        CHECK(queue_enqueue(&queue, &vals[i]) == CSTRUCT_SUCCESS);
    }

    CHECK(queue.data.size == 5);

    // dequeue in the SAME order they were enqueued
    for (int i = 0; i < 5; i++) {
        front = (int*)queue_peek(&queue);
        CHECK(front != NULL && *front == vals[i]);
        CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    }

    CHECK(queue.data.size == 0);

    // Interleaved enqueue/dequeue (the true FIFO check)
    // Enqueue 1, dequeue 1, enqueue 2, dequeue 2 ... size never exceeds 1
    int p = 11, q = 22, r = 33;
    CHECK(queue_enqueue(&queue, &p) == CSTRUCT_SUCCESS);
    CHECK(*(int*)queue_peek(&queue) == 11);
    CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);

    CHECK(queue_enqueue(&queue, &q) == CSTRUCT_SUCCESS);
    CHECK(queue_enqueue(&queue, &r) == CSTRUCT_SUCCESS);
    // q, not r
    CHECK(*(int*)queue_peek(&queue) == 22);
    CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    // r remains
    CHECK(*(int*)queue_peek(&queue) == 33);
    CHECK(queue_dequeue(&queue) == CSTRUCT_SUCCESS);
    CHECK(queue.data.size == 0);

    // Free resets state
    queue_free(&queue);
    CHECK(queue.data.head == NULL);
    CHECK(queue.data.tail == NULL);
    CHECK(queue.data.size == 0);
    queue_free(NULL);

    REPORT();
}
