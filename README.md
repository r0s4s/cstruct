# `cstruct`

A small C library implementing classic data structures.

## Structures

| Structure                     | Prefix        | Type        | Header         |
| -------------------------     | ------------- | ----------- | -------------- |
| **Dynamic Array**             | `dynarray_`   | `DynArray`  | `dynarray.h`   |
| **Linked List**               | `list_`       | `List`      | `list.h`       |
| **Doubly Linked List**        | `dlist_`      | `DList`     | `dlist.h`      |
| **Stack**                     | `stack_`      | `Stack`     | `stack.h`      |
| **Queue**                     | `queue_`      | `Queue`     | `queue.h`      |
| **Deque**                     | `deque_`      | `Deque`     | `deque.h`      |
| **Hash Map**                  | `hashmap_`    | `HashMap`   | `hashmap.h`    |
| **Hash Set**                  | `hashset_`    | `HashSet`   | `hashset.h`    |
| **Binary Tree**               | `bintree_`    | `BinTree`   | `bintree.h`    |
| **Binary Search Tree**        | `bst_`        | `BST`       | `bst.h`        |
| **Heap**/**Priority Queue**   | `heap_`       | `Heap`      | `heap.h`       |
| **Graph**                     | `graph_`      | `Graph`     | `graph.h`      |
| **Union-Find**                | `unionfind_`  | `UnionFind` | `unionfind.h`  |
| **Trie**                      | `trie_`       | `Trie`      | `trie.h`       |

> **Note**: *Work in progress*

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

- **Generic via `void *` and `element_size`**.
- **Short, uniform prefixes** (e.g. `dynarray_init`, `list_insert_at_end`, etc.).
- **Single error enum** (`ErrorCode`) from `error.h` used throughout the library.
- **Validate before mutate** (functions perform fallible work like validation, overflow checks, allocation, etc. *before* touching the struct).
- **No hidden `exit()`** (the library reports failures, the caller decides policy).

## Error Codes

| Code                      | Meaning                                 |
| ------------------------- | --------------------------------------- |
| `CSTRUCT_SUCCESS`         | Operation succeeded                     |
| `CSTRUCT_ERR_INVALID_ARG` | Invalid argument (e.g. `NULL`, size 0)  |
| `CSTRUCT_ERR_OVERFLOW`    | `size_t` arithmetic would overflow      |
| `CSTRUCT_ERR_ALLOC`       | `malloc`/`realloc` failed               |
| `CSTRUCT_ERR_UNDERFLOW`   | Operation on empty container            |

## Usage

```c
#include <stdio.h>

#include "dynarray.h"

int main(void) {
    DynArray array;
    if (dynarray_init(&array, 4, sizeof(int)) != CSTRUCT_SUCCESS) {
      return 1;
    }

    int a = 10;
    int b = 20;
    dynarray_insert(&array, &a);
    dynarray_insert(&array, &b);

    for (size_t i = 0; i < array.size; i++) {
      printf("%d\n", *(int *)dynarray_get(&array, i));
    }

    dynarray_free(&array);
    return 0;
}
```

## Build and Test

```bash
make                      # Build all test binaries
make build/test_dynarray  # Build one
make test                 # Build and run everything
make clean
```

## License

MIT - see [LICENSE](LICENSE).
