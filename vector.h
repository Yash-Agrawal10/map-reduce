#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

// Vector implementation for char*
typedef struct vector vector;

// Vector constructor (and wrapper)
vector* vector_create(int capacity);

// Vector destructor
void vector_destroy(vector* vec);

// Insert char* into vector
void vector_insert(vector* vec, char* value);

// Get specific index in vector
void vector_get(vector* vec, int index);

// Set value in vector
void vector_set(vector* vec, size_t index, int value);

// Sort values in vector (alphabetically for char*)
void vector_sort(vector* vec);


// Vector iterator
typedef struct vector_it vector_it;

// Interator constructor
vector_it* vector_it_create(vector* vec);

// Iterator destructor (doesn't destroy internal vec)
void vector_it_destroy(vector_it* it);

// Iterator is end
int vector_it_end(vector_it* it);

// Iterator next
char* vector_it_next(vector_it* it);

#endif