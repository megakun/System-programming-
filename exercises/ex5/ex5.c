// ex5
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/05/20
// Copyright[2019]<Haokun Cai>

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Vector.h"

#define INITIAL_SIZE 4  // Inital vector length
#define FINAL_SIZE 10  // Targeted vector length.

static void PrintIntVector(vector_t v);

int main(int argc, char *argv[]) {
  size_t i;
  vector_t v = Vector_Allocate(INITIAL_SIZE);

  if (v == NULL) {
    return EXIT_FAILURE;
  }
  for (i = 0; i < FINAL_SIZE; ++i) {  // Place some elements in the vector.
    int *x = (int*)malloc(sizeof(int));
    if (x == NULL) {
      return EXIT_FAILURE;
    }
    *x = FINAL_SIZE - i;
    element_t old;

    if (!(Vector_Set(v, i, x, &old))) {  // test if vector modified fits
      return EXIT_FAILURE;
    }
  }

  PrintIntVector(v);

  // free the vector
  for (i = 0; i < Vector_Length(v); ++i) {
    free(Vector_Get(v, i));
  }
  Vector_Free(v);

  return EXIT_SUCCESS;
}

// helper function to print vector
static void PrintIntVector(vector_t v) {
  assert(v != NULL);

  size_t i;
  size_t length;

  length = Vector_Length(v);

  if (length > 0) {
    printf("[%d", *((int*)Vector_Get(v, 0)));
    for (i = 1; i < Vector_Length(v); ++i)
      printf(",%d", *((int*)Vector_Get(v, i)));
    printf("]\n");
  }
}
