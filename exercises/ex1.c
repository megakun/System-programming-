// ex1
// Haokun Cai
// haokun@uw.edu
// 1832787
// 06/25/19
// Copyright[2019]<Haokun Cai>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#define SIZE 11  // constant for array length

// Helper method for taking two array and length
void CopyAndSort(int16_t *src, int16_t *dst, int len);

// Helper method for insertion sorting
void insSort(int16_t *arr, int num, int len);

// main function
int main(int argc, char **argv) {
  int16_t unsorted[SIZE] = {3, 2, -5, 8, 17, 6, 1, 7, 8, -8, 6};
  int16_t sorted[SIZE];

  CopyAndSort(unsorted, sorted, SIZE);

  for (int i = 0; i < SIZE; i++) {
    printf("%" PRId16, sorted[i]);
    printf(" ");
  }
  printf("%" PRId16 "\n", sorted[SIZE - 1]);

  return EXIT_SUCCESS;
}

// adding element from src array to dst array one by one,
// insertion sort in each addition
void CopyAndSort(int16_t *src, int16_t *dst, int len) {
  for (int i = 0; i < len; i++) {
    insSort(dst, src[i], i);
  }
}

// insertion sort
void insSort(int16_t *arr, int num, int len) {
  int i = len;
  while (i > 0 && arr[i-1] > num) {
    arr[i] = arr[i-1];
    i--;
  }
  arr[i] = num;
}
