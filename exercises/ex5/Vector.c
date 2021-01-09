// ex5
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/05/20
// Copyright[2019]<Haokun Cai>

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Vector.h"

// Returns a copy of arry with new length newLen.  If newLen < oldLen
// then the returned array is clipped.  If newLen > oldLen, then the
// resulting array will be padded with NULL elements.
static element_t *ResizeArray(element_t *arry, size_t oldLen, size_t newLen);

vector_t Vector_Allocate(size_t n) {
  assert(n > 0);

  vector_t v = (vector_t)malloc(sizeof(struct vector_st));
  if (v == NULL) {
    return NULL;
  }

  v->arry = (element_t*)malloc(n*sizeof(element_t));
  if (v->arry == NULL) {
    return NULL;
  }

  v->length = n;
  for (size_t i = 0; i < n; ++i) {
    v->arry[i] = NULL;
  }

  return v;
}

void Vector_Free(vector_t v) {
  assert(v != NULL);
  free(v->arry);
  free(v);
}

bool Vector_Set(vector_t v, int index, element_t e, element_t *prev) {
  assert(v != NULL);
  assert(index >= 0);

  if (index >= v->length) {
    size_t newLength = index+1;
    element_t *newArray = ResizeArray(v->arry, v->length, newLength);
    if (newArray == NULL) {
      return false;
    }
    free(v->arry);
    v->arry = newArray;
    v->length = newLength;
  }
  prev = v->arry[index];
  v->arry[index] = e;

  return true;
}

element_t Vector_Get(vector_t v, int index) {
  assert(v != NULL);
  assert(index >= 0);
  return v->arry[index];
}

size_t Vector_Length(vector_t v) {
  assert(v != NULL);
  return v->length;
}

static element_t *ResizeArray(element_t *arry, size_t oldLen, size_t newLen) {
  assert(arry != NULL);
  assert(newLen > 0);

  size_t i;
  size_t copyLen = oldLen > newLen ? newLen : oldLen;
  element_t *newArry;

  newArry = (element_t*)malloc(newLen*sizeof(element_t));

  if (newArry == NULL)
    return NULL;  // malloc error!!!

  // Copy elements to new array
  for (i = 0; i < copyLen; ++i)
    newArry[i] = arry[i];

  // Null initialize rest of new array.
  for (i = copyLen; i < newLen; ++i)
    newArry[i] = NULL;

  return newArry;
}
