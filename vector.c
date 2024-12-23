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

// Vector Constructor / Destructor
vector* vector_create(int capacity){
    assert(capacity > 0);
    vector* vec = malloc(sizeof(vector));
    assert(vec != NULL);
    vec->data = malloc(sizeof(char*) * capacity);
    assert(vec->data != NULL);
    vec->size = 0;
    vec->capacity = capacity;
    return vec;
}

void vector_destroy(vector* vec){
    assert(vec != NULL);
    for (int i = 0; i < vec->size; i++){
        free(vec->data[i]);
    }
    free(vec->data);
    free(vec);
}

// Vector operations
void vector_insert(vector* vec, char* value){
    if (vec->size == vec->capacity){
        size_t old_cap = vec->capacity;
        vec->capacity *= 2;
        assert(vec->capacity > old_cap);
        vec->data = realloc(vec->data, vec->capacity * sizeof(char*));
        assert(vec->data != NULL);
    }
    vec->data[vec->size] = strdup(value);
    vec->size++;
}

char* vector_get(vector* vec, size_t index){
    if (index >= vec->size){
        fprintf(stderr, "Index out of bounds\n");
        exit(1);
    }
    return vec->data[index];
}

void vector_set(vector* vec, size_t index, char* value){
    if (index >= vec->size){
        fprintf(stderr, "Index out of bounds\n");
        exit(1);
    }
    free(vec->data[index]);
    vec->data[index] = strdup(value);
}

int cmp(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void vector_sort(vector* vec){
    qsort(vec->data, vec->size, sizeof(char*), cmp);
}

// Iterator operations
vector_it vector_it_create(vector* vec){
    assert(vec != NULL);
    vector_it it = {vec, 0};
    return it;
}

int vector_it_end(const vector_it* it){
if (it->curr_index < it->vec->size){
        return 0;
    }
    else{
        return 1;
    }
}

char* vector_it_next(vector_it* it){
    char* rc = vector_get(it->vec, it->curr_index);
    it->curr_index++;
    return rc;
}