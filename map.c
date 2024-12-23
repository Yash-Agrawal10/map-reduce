#include "map.h"

// Structs
typedef struct bucket_t {
    char* key;
    char** values;
    int values_count;
    int values_cap;
    struct node_t* next;
} bucket_t;

typedef struct map_t {
    char** keys;
    int keys_count;
    int keys_cap;
    bucket_t buckets[NUM_BUCKETS];
};

// Map Functions


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
