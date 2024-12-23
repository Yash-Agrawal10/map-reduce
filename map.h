#ifndef MAP_H
#define MAP_H

#include "vector.h"

#define NUM_BUCKETS 10007

// Hash-map implementation
typedef struct map_t map_t;

// Initialize empty map for use
map_t* map_create();

// Destroy map
void map_destroy(map_t* map);

// Insert kv pair into map
void map_insert(map_t* map, char* key, char* value);

// Sort keys in map
void map_sort(map_t* map);

// Get values corresponding to key from map as iterator
vector_it map_get(map_t* map, char* key);

// Get keys vec as iterator
vector_it map_keys(map_t* map);

#endif