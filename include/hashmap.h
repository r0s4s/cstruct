/* ./cstruct/include/hashmap.h */

#ifndef CSTRUCT_HASHMAP_H
#define CSTRUCT_HASHMAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "error.h"

typedef enum {
    KEY_INT,                // Integer key type
    KEY_STR                 // String key type
} KeyType;

typedef struct Key {
    KeyType type;           // Allows to distinguish the key type at runtime
    union {
        int     as_int;     // Support int keys
        char*   as_str;     // Support string keys
    } data;
} Key;

typedef struct Entry {
    Key     key;            // Entry key
    void*   value;          // Entry value as a void*
    bool    is_occupied;    // If this entry contains live data
    bool    is_tombstone;   // If this entry had a deleted value here
} Entry;

typedef struct HashMap {
    Entry*  entries;        // Flat array of entries
    size_t  size;           // Hash map size
    size_t  capacity;       // Hash map capacity
} HashMap;

uint32_t hash_int(int key);
uint32_t hash_str(const char* key);

ErrorCode hashmap_init(HashMap* map, size_t capacity);
ErrorCode hashmap_insert(HashMap* map, Key key, const void* value);
ErrorCode hashmap_delete(HashMap* map, const Key key);

void* hashmap_lookup(const HashMap* map, const Key key);
void hashmap_free(HashMap* map);

#endif /* CSTRUCT_HASHMAP_H */
