// ex5
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/05/20
// Copyright[2019]<Haokun Cai>

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdbool.h>
#include <stddef.h>

// An arbitrary pointer to represent an element in the vector.
typedef void* element_t;

// An expandable array of element_ts.
typedef struct vector_st {
  size_t length;
  element_t *arry;
} *vector_t;

// On success, return vector_t with an initial length of n.
// On failure, returns NULL.  Assumes v != NULL.
vector_t Vector_Allocate(size_t n);

// Frees the memory allocated for the vector_t.  Assumes v != NULL.
void Vector_Free(vector_t v);

// Sets the nth element of v to be e.  Returns the previous nth element_t in
// prev. Freeing e is the responsibility of the user, not the vector_t.
// Returns true iff successful.  Assumes v != NULL.
bool Vector_Set(vector_t v, int index, element_t e, element_t *prev);

// Returns the element at the given index within v.  Assumes v != NULL.
element_t Vector_Get(vector_t v, int index);

// Returns the length of v.  Assumes v != NULL.
size_t Vector_Length(vector_t v);

#endif  // _VECTOR_H_
