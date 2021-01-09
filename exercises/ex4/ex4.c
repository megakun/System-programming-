// ex4
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/05/20
// Copyright[2019]<Haokun Cai>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "./NthPrime.h"

int main(int argc, char **argv) {
  printf("The 10th prime is: %" PRIu64 "\n", NthPrime(10));
  printf("The 100th prime is: %" PRIu64 "\n", NthPrime(100));
  printf("The 1000th prime is: %" PRIu64 "\n", NthPrime(1000));
  return EXIT_SUCCESS;
}
