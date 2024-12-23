#ifndef MAP_H
#define MAP_H

#include "vector.h"

#define NUM_BUCKETS 10007

// Hash-map implementation
typedef struct map_t map_t;

// Initialize empty map for use
void map_init(map_t* map);

// Insert kv pair into map
void map_insert(map_t* map, char* key, char* value);

// Get values corresponding to key from map
vector* map_get(map_t* map, char* key);

// Get sorted list of keys in map
vector* map_keys(map_t* map);

#endif