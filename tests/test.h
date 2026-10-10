/* ./cstruct/tests/test.h */

#ifndef CSTRUCT_TEST_H
#define CSTRUCT_TEST_H

#include <stdio.h>

static int test_failures = 0;

#define CHECK(condition)                                            \
    do {                                                            \
        if (!(condition)) {                                         \
            printf("  FAIL: %s (line %d)\n", #condition, __LINE__); \
            test_failures++;                                        \
        }                                                           \
    } while (0)

#define REPORT()                                                    \
    do {                                                            \
        if (test_failures == 0) {                                   \
            printf("All tests passed.\n");                          \
            return 0;                                               \
        }                                                           \
        printf("%d test(s) failed.\n", test_failures);              \
        return 1;                                                   \
    } while (0)

#endif /* CSTRUCT_TEST_H */
