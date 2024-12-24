#include "mapreduce.h"
#include "map.h"
#include "vector.h"
#include <pthread.h>
#include <string.h>

// Global state variable
typedef struct state_t {
    Partitioner partition_func;
    int num_partitions;
    map_t** partitions;
    pthread_mutex_t** locks;
} state_t;

static state_t* state_g;

// Argument structs
typedef struct mapper_arg_t {
    int* index;
    int argc;
    char** argv;
    Mapper map_func;
    pthread_mutex_t lock;
} mapper_arg_t;

typedef struct reducer_arg_t {
    Reducer reducer_func;
    Getter get_func;
    int partition_number;
} reducer_arg_t;

// Internal Function Implementations
char* get_next(char* key, int partition_number){
    map_t* map = state_g->partitions[partition_number];
    return map_get_next(map, key);
}

// Thread functions
void* mapper(void* arg){
    // Parse arguments
    mapper_arg_t* args = (mapper_arg_t*) arg;
    Mapper map_func = args->map_func;
    char** argv = args->argv;
    int argc = args->argc;
    int* index = args->index;
    pthread_mutex_t lock = args->lock;

    // Loop over files, trying to claim them
    while (1){
        pthread_mutex_lock(&lock);
        if (*index >= argc){
            pthread_mutex_unlock(&lock);
            return NULL;
        }
        else{
            char* filename = argv[*index];
            *index++;
            pthread_mutex_unlock(&lock);
            map_func(filename);
        }
    }

    // Return once files are all consumed
    return NULL;
}

void* reducer(void* arg){
    // Parse arguments
    reducer_arg_t* args = (reducer_arg_t*) arg;
    Reducer reducer_func = args->reducer_func;
    Getter get_func = args->get_func;
    int partition_number = args->partition_number;
    map_t* map = state_g->partitions[partition_number];

    // Prepare map for reducing
    map_sort(map);
    map_init_its(map);

    // Call reducer function for each key
    vector_it keys = map_keys(map);
    while (!vector_it_end(&keys)){
        char* key = vector_it_next(&keys);
        reducer_func(key, get_next, partition_number);
    }

    return NULL;
}

// External Function Implementations
void state_g_init(int num_reducers, Partitioner partition){
    int num_partitions = num_reducers;
    map_t** partitions = malloc(sizeof(map_t*) * num_partitions);
    assert(partitions != NULL);
    pthread_mutex_t** locks = malloc(sizeof(pthread_mutex_t*) * num_partitions);
    assert(locks != NULL);
    for (int i = 0; i < num_partitions; i++){
        assert(pthread_mutex_init(locks[i], NULL) == 0);
    }
    state_g->locks = locks;
    state_g->num_partitions = num_partitions;
    state_g->partition_func = partition;
    state_g->partitions = partitions;
}

void MR_Emit(char *key, char *value){
    int partition = state_g->partition_func(key, state_g->num_partitions);
    pthread_mutex_lock(state_g->locks[partition]);
    map_insert(state_g->partitions[partition], key, value);
    pthread_mutex_unlock(state_g->locks[partition]);
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
    // Initialize global state
    state_g_init(num_reducers, partition);

    // Map
    int index = 1;
    pthread_mutex_t lock;
    int rc = pthread_mutex_init(&lock, NULL);
    assert(rc == 0);
    pthread_t mappers[num_mappers];
    mapper_arg_t mapper_args[num_mappers];
    for (int i = 0; i < num_mappers; i++){
        mapper_arg_t mapper_arg = {
            &index, argc, 
            argv, map, lock
        };
        mapper_args[i] = mapper_arg;
        pthread_create(&mappers[i], NULL, mapper, &mapper_args[i]);
    }
    for (int i = 0; i < num_mappers; i++){
        pthread_join(mappers[i], NULL);
    }
    

    // Reduce
    pthread_t reducers[num_reducers];
    reducer_arg_t reducer_args[num_reducers];
    for (int i = 0; i < num_reducers; i++){
        reducer_arg_t reducer_arg = {
            reduce, 
            get_next, 
            i
        };
        reducer_args[i] = reducer_arg;
        pthread_create(&reducers[i], NULL, reducer, &reducer_args[i]);
    }
    for (int i = 0; i < num_mappers; i++){
        pthread_join(reducers[i], NULL);
    }

    exit(0);
}