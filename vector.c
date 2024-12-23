#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

// Vector struct
typedef struct vector {
    char** data;
    size_t size;
    size_t capacity;
} vector;

// Constructor / Destructor
void vector_init(vector* vec, int capacity){
    vec->data = malloc(sizeof(char*) * capacity);
    assert(vec->data != NULL);
    vec->size = 0;
    vec->capacity = capacity;
}

void vector_destroy(vector* vec){
    free(vec->data);
    free(vec);
}

// Vector operations
void vector_insert(vector* vec, char* value){
    if (vec->size == vec->capacity){
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->capacity * sizeof(int));
        assert(vec->data != NULL);
    }
    vec->data[vec->size] = value;
    vec->size++;
}

int vector_get(vector* vec, size_t index){
    if (index >= vec->size){
        fprintf(stderr, "Index out of bounds\n");
        exit(1);
    }
    return vec->data[index];
}

void vector_set(vector* vec, size_t index, int value){
    if (index >= vec->size){
        fprintf(stderr, "Index out of bounds\n");
        exit(1);
    }
    vec->data[index] = value;
}

int cmp(const void* a, const void* b) {
    return strcp(*(const char**)a, *(const char**)b);
}

void vector_sort(vector* vec){
    qsort(vec->data, vec->size, sizeof(char*), cmp);
}

// Iterator struct
typedef struct vector_it {
    vector* vec;
    size_t curr_index;
} vector_it;

// Iterator operations
int vector_it_end(vector_it* it){
    if (it->curr_index < it->vec->size){
        return 0;
    }
    return 1;
}

char* vector_it_next(vector_it* it){
    char* rc = vector_get(it->vec, it->curr_index);
    it->curr_index++;
    return rc;
}