#include "map.h"
#include "vector.h"
#include <string.h>

// Structs
typedef struct bucket_t {
    char* key;
    vector* values;
    struct node_t* next;
} bucket_t;

typedef struct map_t {
    vector* keys;
    bucket_t* buckets[NUM_BUCKETS];
} map_t;

// Hashing
unsigned long get_hash(const char* str){
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

int get_bucket(const char* str){
    int hash = get_hash(str);
    int bucket = hash % NUM_BUCKETS;
    return bucket;
}

// Bucket operations
bucket_t* bucket_create(char* key, vector* values){
    bucket_t* bucket = malloc(sizeof(bucket_t));
    assert(bucket != NULL);
    bucket->key = strdup(key);
    bucket->values = values;
    bucket->next = NULL;
    return bucket;
}

void bucket_destroy(bucket_t* bucket){
    while (bucket != NULL){
        free(bucket->key);
        vector_destroy(bucket->values);
        bucket_t* to_free = bucket;
        bucket = bucket->next;
        free(to_free);
    }
}

// Map Internal Functions
vector* get_values(map_t* map, char* key){
    int bucket_num = get_bucket(key);
    bucket_t* bucket = map->buckets[bucket_num];
    while (bucket != NULL){
        if (strcmp(bucket->key, key) == 0){
            return bucket->values;
        }
        else{
            bucket = bucket->next;
        }
    }
    return NULL;
}

// Map External Functions
map_t* map_create(){
    map_t* map = malloc(sizeof(map_t));
    assert(map != NULL);
    map->keys = vector_create(1);
    memset(map->buckets, 0, sizeof(map->buckets));
    return map;
}

void map_destroy(map_t* map){
    for (int i = 0; i < NUM_BUCKETS; i++){
        bucket_destroy(map->buckets[i]);
    }
    vector_destroy(map->keys);
}

void map_insert(map_t* map, char* key, char* value){
    vector* values = get_values(map, key);
    if (values != NULL){
        vector_insert(values, value);
    }
    else{
        values = vector_create(1);
        vector_insert(values, value);

        bucket_t* new_bucket = bucket_create(key, values);
        int bucket_num = get_bucket(key);
        new_bucket->next = map->buckets[bucket_num];
        map->buckets[bucket_num] = new_bucket;
        vector_insert(map->keys, key);
    }
}

void map_sort(map_t* map){
    vector_sort(map->keys);
}

vector_it map_get(map_t* map, char* key){
    vector* values = get_values(map, key);
    assert(values != NULL);
    vector_it it = vector_it_create(values);
    return it;
}

vector_it map_keys(map_t* map){
    vector_it it = vector_it_create(map->keys);
    return it;
}
