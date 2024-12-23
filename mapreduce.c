#include "mapreduce.h"
#include <pthread.h>

// Global state variable
typedef struct state_t {
    Partitioner partition;
    char** kv_map;
    pthread_mutex_t lock;
} state_t;

static state_t* state_g;

// External Function Implementations

void MR_Emit(char *key, char *value){
    int index = 
    pthread_mutex_lock(&state_g->lock);
}

unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}

void MR_Run(int argc, char *argv[], 
            Mapper map, int num_mappers, 
            Reducer reduce, int num_reducers, 
            Partitioner partition){
    
}