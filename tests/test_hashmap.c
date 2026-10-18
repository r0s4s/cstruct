/* ./cstruct/tests/test_hashmap.c */

#include "test.h"
#include "hashmap.h"

static Key k_int(int v) {
    Key k;
    k.type = KEY_INT;
    k.data.as_int = v;
    return k;
}

static Key k_str(const char* v) {
    Key k;
    k.type = KEY_STR;
    k.data.as_str = (char*)v;
    return k;
}

int main(void) {
    HashMap map;

    // Invalid init
    CHECK(hashmap_init(NULL, 4) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_init(&map, 0) == CSTRUCT_ERR_INVALID_ARG);

    // Valid init
    CHECK(hashmap_init(&map, 8) == CSTRUCT_SUCCESS);
    CHECK(map.size == 0);
    CHECK(map.capacity == 8);
    CHECK(map.entries != NULL);

    // Empty-map lookups and deletes
    CHECK(hashmap_lookup(&map, k_int(42)) == NULL);
    CHECK(hashmap_lookup(&map, k_str("hello")) == NULL);
    CHECK(hashmap_delete(&map, k_int(42)) == CSTRUCT_ERR_NOT_FOUND);
    CHECK(hashmap_delete(&map, k_str("hello")) == CSTRUCT_ERR_NOT_FOUND);

    // Basic int key
    int v100 = 100;
    CHECK(hashmap_insert(&map, k_int(1), &v100) == CSTRUCT_SUCCESS);
    CHECK(map.size == 1);
    CHECK(hashmap_lookup(&map, k_int(1)) == &v100);
    CHECK(hashmap_lookup(&map, k_int(2)) == NULL);

    // Basic string key
    int v200 = 200;
    CHECK(hashmap_insert(&map, k_str("alice"), &v200) == CSTRUCT_SUCCESS);
    CHECK(map.size == 2);
    CHECK(hashmap_lookup(&map, k_str("alice")) == &v200);
    CHECK(hashmap_lookup(&map, k_str("bob")) == NULL);

    // Same numeric value, different key types are distinct
    int v1  = 1, vOne = 1000;
    CHECK(hashmap_insert(&map, k_int(7), &v1) == CSTRUCT_SUCCESS);
    CHECK(hashmap_insert(&map, k_str("7"), &vOne) == CSTRUCT_SUCCESS);
    CHECK(map.size == 4);
    CHECK(hashmap_lookup(&map, k_int(7)) == &v1);
    CHECK(hashmap_lookup(&map, k_str("7")) == &vOne);

    // Update existing keys
    int v101 = 101, v201 = 201;
    CHECK(hashmap_insert(&map, k_int(1), &v101) == CSTRUCT_SUCCESS);
    CHECK(hashmap_insert(&map, k_str("alice"), &v201) == CSTRUCT_SUCCESS);
    CHECK(map.size == 4);
    CHECK(hashmap_lookup(&map, k_int(1)) == &v101);
    CHECK(hashmap_lookup(&map, k_str("alice")) == &v201);

    // Delete int key
    CHECK(hashmap_delete(&map, k_int(1)) == CSTRUCT_SUCCESS);
    CHECK(map.size == 3);
    CHECK(hashmap_lookup(&map, k_int(1)) == NULL);

    // Double-delete returns NOT_FOUND
    CHECK(hashmap_delete(&map, k_int(1)) == CSTRUCT_ERR_NOT_FOUND);

    // String key still there after unrelated delete
    CHECK(hashmap_lookup(&map, k_str("alice")) == &v201);

    // Delete string key
    CHECK(hashmap_delete(&map, k_str("alice")) == CSTRUCT_SUCCESS);
    CHECK(map.size == 2);
    CHECK(hashmap_lookup(&map, k_str("alice")) == NULL);
    CHECK(hashmap_delete(&map, k_str("alice")) == CSTRUCT_ERR_NOT_FOUND);

    // Invalid arguments
    CHECK(hashmap_insert(NULL, k_int(1), &v100) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_insert(&map, k_int(1), NULL) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_insert(&map, k_str(NULL), &v100) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_delete(NULL, k_int(1)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_delete(&map, k_str(NULL)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_lookup(NULL, k_int(1)) == NULL);
    CHECK(hashmap_lookup(&map, k_str(NULL)) == NULL);

    // Load factor growth,
    // 20 inserts force at least two resizes
    int vals[20];
    for (int i = 0; i < 20; i++) {
        vals[i] = i * 10;
        CHECK(hashmap_insert(&map, k_int(i), &vals[i]) == CSTRUCT_SUCCESS);
    }

    CHECK(map.size > 20);
    CHECK(map.capacity > 8);
    for (int i = 0; i < 20; i++) {
        CHECK(hashmap_lookup(&map, k_int(i)) == &vals[i]);
    }

    // Tombstone crossing deletes every other int key,
    // Verify survivors
    for (int i = 0; i < 20; i += 2) {
        CHECK(hashmap_delete(&map, k_int(i)) == CSTRUCT_SUCCESS);
    }

    for (int i = 0; i < 20; i++) {
        if (i % 2 == 0) {
            CHECK(hashmap_lookup(&map, k_int(i)) == NULL);
        } else {
            CHECK(hashmap_lookup(&map, k_int(i)) == &vals[i]);
        }
    }

    // Insert after deletes
    int v_new = 999;
    CHECK(hashmap_insert(&map, k_int(100), &v_new) == CSTRUCT_SUCCESS);
    CHECK(hashmap_lookup(&map, k_int(100)) == &v_new);

    // Mixed string keys after resizes
    int vs1 = 1, vs2 = 2;
    CHECK(hashmap_insert(&map, k_str("foo"), &vs1) == CSTRUCT_SUCCESS);
    CHECK(hashmap_insert(&map, k_str("bar"), &vs2) == CSTRUCT_SUCCESS);
    CHECK(hashmap_lookup(&map, k_str("foo")) == &vs1);
    CHECK(hashmap_lookup(&map, k_str("bar")) == &vs2);

    // Free resets state
    hashmap_free(&map);
    CHECK(map.entries == NULL);
    CHECK(map.size == 0);
    CHECK(map.capacity == 0);

    // Post-free: capacity guardrails kick in
    CHECK(hashmap_insert(&map, k_int(1), &v100) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_delete(&map, k_int(1)) == CSTRUCT_ERR_INVALID_ARG);
    CHECK(hashmap_lookup(&map, k_int(1)) == NULL);

    // Free(NULL) is safe
    hashmap_free(NULL);

    // Re-init after free works cleanly
    CHECK(hashmap_init(&map, 4) == CSTRUCT_SUCCESS);
    CHECK(hashmap_insert(&map, k_int(5), &v100) == CSTRUCT_SUCCESS);
    CHECK(hashmap_lookup(&map, k_int(5)) == &v100);
    hashmap_free(&map);

    REPORT();
}
