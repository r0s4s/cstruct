/* ./cstruct/src/hashmap.c */

#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

#define HASHMAP_LOAD_FACTOR_NUM 7
#define HASHMAP_LOAD_FACTOR_DEN 10

uint32_t hash_int(int key) {
    uint32_t x = (uint32_t)key;

    // Round 1
    x ^= x >> 16;
    x *= 0x85ebca6b;

    // Round 2
    x ^= x >> 13;
    x *= 0xc2b2ae35;

    // Round 3
    x ^= x >> 16;

    return x;
}

uint32_t hash_str(const char* key) {
    const char* s = key;

    // FNV offset basis
    uint32_t hash = 2166136261U;

    // Walk the string one byte at a time until the null terminator
    for (; *s; s++) {
        // XOR the current byte into the low bits of the hash,
        // The cast to unsigned char is essential
        hash ^= (uint32_t)(unsigned char)*s;

        // Multiply by the FNV prime (2^24 + 2^8 + 0x93),
        // Spread the bits across the 32-bit space so small input changes cascade widely
        hash *= 16777619U;
    }

    return hash;
}

static uint32_t hashmap_hash(const Key key) {
    switch (key.type) {
        case KEY_INT:
            return hash_int(key.data.as_int);
        case KEY_STR:
            return hash_str(key.data.as_str);
        default:
            // Undefined behavior safety fallback
            return 0;
    }
}

static bool hashmap_keys_match(const Entry* entry, const Key key) {
    // Check if the keys have the same type
    if (entry->key.type != key.type) {
        return false;
    }

    // Check if the key values are equal
    if (key.type == KEY_INT) {
        return entry->key.data.as_int == key.data.as_int;
    } else {
        if (entry->key.data.as_str != NULL) {
            return strcmp(entry->key.data.as_str, key.data.as_str) == 0;
        } else {
            // The enry key data string is NULL
            return false;
        }
    }
}

ErrorCode hashmap_init(HashMap* map, size_t capacity) {
    // Check for invalid parameter values
    if (map == NULL || capacity == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Prevent capacity * sizeof(Entry) from overflowing size_t
    if (capacity > SIZE_MAX / sizeof(Entry)) {
        return CSTRUCT_ERR_OVERFLOW;
    }

    // Allocate new entries based on the desired capacity
    void *new_entries = calloc(capacity, sizeof(Entry));
    if (new_entries == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    // Initialize the hash map
    map->entries    = new_entries;
    map->size       = 0;
    map->capacity   = capacity;

    return CSTRUCT_SUCCESS;
}

static ErrorCode hashmap_insert_at(HashMap* map, size_t index, Key* key, const void* value) {
    Entry* target = &map->entries[index];

    if (key->type == KEY_STR) {
        size_t len = strlen(key->data.as_str) + 1;

        char* copied = malloc(len);
        if (copied == NULL) {
            return CSTRUCT_ERR_ALLOC;
        }

        memcpy(copied, key->data.as_str, len);
        target->key.data.as_str = copied;
    } else {
        target->key.data.as_int = key->data.as_int;
    }

    // Update target entry
    target->key.type     = key->type;
    target->value        = (void*)value;
    target->is_occupied  = true;
    target->is_tombstone = false;

    // Increment hash map size
    map->size++;

    return CSTRUCT_SUCCESS;
}

static ErrorCode hashmap_resize(HashMap* map) {
    // Doubling the hash map capacity
    size_t new_capacity = map->capacity * 2;

    // Check for overflow
    if (new_capacity < map->capacity) {
        return CSTRUCT_ERR_OVERFLOW;
    }

    // Check for overflow
    if (new_capacity > SIZE_MAX / sizeof(Entry)) {
        return CSTRUCT_ERR_OVERFLOW;
    }

    // Allocate new entries
    Entry* new_entries = calloc(new_capacity, sizeof(Entry));
    if (new_entries == NULL) {
        return CSTRUCT_ERR_ALLOC;
    }

    for (size_t i = 0; i < map->capacity; i++) {
        Entry* old = &map->entries[i];
        if (!old->is_occupied) {
            // Skip empty slots and tombstones
            continue;
        }

        uint32_t rehash = hashmap_hash(old->key);
        size_t index = rehash % new_capacity;

        // Linear probe in the new table until an empty slot is found
        while (new_entries[index].is_occupied) {
            index = (index + 1) % new_capacity;
        }

        new_entries[index] = *old;
    }

    // Free old entries
    free(map->entries);

    // Update the hash map
    map->entries  = new_entries;
    map->capacity = new_capacity;

    return CSTRUCT_SUCCESS;
}

ErrorCode hashmap_insert(HashMap* map, Key key, const void* value) {
    // Check for invalid parameter values
    if (map == NULL || value == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // If the capacity is 0,
    // This leads to unexpected behaviors
    if (map->capacity == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Check for invalid key type
    if (key.type == KEY_STR && key.data.as_str == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Resize before probing based on the load factor
    if ((map->size + 1) * HASHMAP_LOAD_FACTOR_DEN
        > map->capacity * HASHMAP_LOAD_FACTOR_NUM) {
        ErrorCode resize_code = hashmap_resize(map);
        if (resize_code != CSTRUCT_SUCCESS) {
            // Pass the resize error code
            return resize_code;
        }
    }

    uint32_t hash = hashmap_hash(key);
    size_t index = hash % map->capacity;

    // Probe for either a matching key or a free slot,
    // first_tombstone lets us reuse a slot left by a previous delete,
    // Without breaking the probe chain for other keys
    size_t first_tombstone = SIZE_MAX;

    for (size_t i = 0; i < map->capacity; i++) {
        Entry* entry = &map->entries[index];

        if (entry->is_occupied) {
            if (hashmap_keys_match(entry, key)) {
                entry->value = (void*)value;
                return CSTRUCT_SUCCESS;
            }
        } else if (entry->is_tombstone) {
            if (first_tombstone == SIZE_MAX) {
                first_tombstone = index;
            }
        } else {
            // The slot is truly empty
            size_t target = (first_tombstone != SIZE_MAX) ? first_tombstone : index;
            return hashmap_insert_at(map, target, &key, value);
        }

        index = (index + 1) % map->capacity;
    }

    // Probe exhausted if the tombstone is seen reuse it,
    // Otherwise the map is genuinely full
    if (first_tombstone != SIZE_MAX) {
        return hashmap_insert_at(map, first_tombstone, &key, value);
    }

    return CSTRUCT_ERR_OVERFLOW;
}

ErrorCode hashmap_delete(HashMap* map, const Key key) {
    // Check for invalid parameter value
    if (map == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // If the capacity is 0,
    // This leads to unexpected behaviors
    if (map->capacity == 0) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Check for invalid key type
    if (key.type == KEY_STR && key.data.as_str == NULL) {
        return CSTRUCT_ERR_INVALID_ARG;
    }

    // Compute hash
    uint32_t hash = hashmap_hash(key);

    // Probe for an empty slot or a matching key
    size_t index = hash % map->capacity;
    size_t probe_count = 0;

    while (true) {
        Entry* entry = &map->entries[index];

        // Check if the slot is occupied
        if (entry->is_occupied) {
            if (hashmap_keys_match(entry, key)) {
                // Update the slot and delete the value
                entry->value = NULL;

                // Free the string key
                if (entry->key.type == KEY_STR) {
                    free(entry->key.data.as_str);
                    entry->key.data.as_str = NULL;
                }

                entry->is_occupied  = false;
                entry->is_tombstone = true;

                // Decrement the hash map size
                map->size--;

                return CSTRUCT_SUCCESS;
            }
        } else if (!entry->is_tombstone) {
            // The slot is truly empty
            return CSTRUCT_ERR_NOT_FOUND;
        }

        index = (index + 1) % map->capacity;
        probe_count++;

        // The hash map is full
        if (probe_count >= map->capacity) {
            return CSTRUCT_ERR_NOT_FOUND;
        }
    }
}

void* hashmap_lookup(const HashMap* map, const Key key) {
    // Check for invalid parameter value
    if (map == NULL) {
        return NULL;
    }

    // If the capacity is 0,
    // This leads to unexpected behaviors
    if (map->capacity == 0) {
        return NULL;
    }

    // Check for invalid key type
    if (key.type == KEY_STR && key.data.as_str == NULL) {
        return NULL;
    }

    // Compute hash
    uint32_t hash = hashmap_hash(key);

    // Probe for an empty slot or a matching key
    size_t index = hash % map->capacity;
    size_t probe_count = 0;

    while (true) {
        Entry* entry = &map->entries[index];

        if (entry->is_occupied) {
            if (hashmap_keys_match(entry, key)) {
                return entry->value;
            }
        } else if (!entry->is_tombstone) {
            // The slot is truly empty
            return NULL;
        }

        index = (index + 1) % map->capacity;
        probe_count++;

        // The hash map is full
        if (probe_count >= map->capacity) {
            return NULL;
        }
    }

    // Return NULL if key is not found
    return NULL;
}

void hashmap_free(HashMap* map) {
    // Check for invalid parameter value
    if (map == NULL) {
        return;
    }

    // Iterate over the entries
    for (size_t i = 0; i < map->capacity; i++) {
        Entry* entry = &map->entries[i];

        // Check to free string keys
        if (entry->is_occupied && entry->key.type == KEY_STR) {
            free(entry->key.data.as_str);
        }
    }

    // Free the hash map and clear its state
    free(map->entries);
    map->entries  = NULL;
    map->size     = 0;
    map->capacity = 0;
}
