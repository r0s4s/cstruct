# `cstruct`

A small C library implementing classic data structures.

> **Note**: *Work in progress*

## Structures

- [x] **Dynamic Array**
- [x] **Linked List**
- [x] **Stack**
- [x] **Queue**
- [ ] **Deque**
- [ ] **Hash Map**
- [ ] **Hash Set**
- [ ] **Binary Tree**
- [ ] **Binary Search Tree**
- [ ] **Heap** / **Priority Queue**
- [ ] **Graph**
- [ ] **Union-Find**
- [ ] **Trie**

## Project Layout

```
cstruct/
├── include/        # Public headers
├── src/            # Implementations
├── tests/          # One test file per structure
├── Makefile
└── README.md
```

## Conventions

- **Generic via `void *` and `element_size`**. The container moves bytes, the
  caller decides what those bytes mean. The library never knows the element type.
- **Prefixed names**. Every public symbol starts with the snake_case name of its
  structure: `dynamic_array_init`, `linked_list_insert_at_end`, avoiding
  collisions and ambiguity at call sites.
- **Single error enum**. All functions return `ErrorCode` from `error.h`.
  `0` is success, negative values are failures. No magic numbers.
- **Validate before mutate**. Functions perform all fallible work (validation,
  overflow checks, allocation) *before* touching the struct. A failed call leaves
  the object untouched.
- **No hidden `exit()`**. The library reports failures, the caller decides policy.

## Error Codes

| Code                      | Meaning                             |
| ------------------------- | ----------------------------------- |
| `CSTRUCT_SUCCESS`         | Operation succeeded                 |
| `CSTRUCT_ERR_INVALID_ARG` | Bad argument (e.g. `NULL`, size 0)  |
| `CSTRUCT_ERR_OVERFLOW`    | `size_t` arithmetic would overflow  |
| `CSTRUCT_ERR_ALLOC`       | `malloc` / `realloc` failed         |
| `CSTRUCT_ERR_UNDERFLOW`   | Operation on empty container        |

## Usage

```c
#include <stdio.h>

#include "dynamic_array.h"

int main(void) {
    DynamicArray array;
    if (dynamic_array_init(&array, 4, sizeof(int)) != CSTRUCT_SUCCESS) {
      return 1;
    }

    int a = 10;
    int b = 20;
    dynamic_array_insert(&array, &a);
    dynamic_array_insert(&array, &b);

    for (size_t i = 0; i < array.size; i++) {
      printf("%d\n", *(int *)dynamic_array_get(&array, i));
    }

    dynamic_array_free(&array);
    return 0;
}
```

## Build and Test

```bash
make                            # Build all test binaries
make build/test_dynamic_array   # Build one
make test                       # Build and run everything
make clean
```

## License

MIT License

Copyright (c) 2026 Konan Marie-William Gabriel

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
