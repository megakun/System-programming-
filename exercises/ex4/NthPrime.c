// ex4
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/05/20
// Copyright[2019]<Haokun Cai>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./NthPrime.h"

bool IsPrime(int64_t n);

int64_t NthPrime(int16_t n) {
  // if n is 0
  if (n == 0) {
    printf("0 can not be passed as a agrument");
    exit(EXIT_FAILURE);
  }

  int64_t next = 2;

  while (1) {
    if (IsPrime(next)) {
      if (n == 1) {  // found the nth prime
        return next;
      }
      n--;  // otherwise decrement n
  }
    // if n is too big
    if (next == INT64_MAX) {
      printf("Reached max possible value");
      exit(EXIT_FAILURE);
    }

    next++;
  }
}

// helper function to check whether a num is prime
bool IsPrime(int64_t n) {
  if (n == 0 || n == 1) {
    return false;
  } else if (n == 2) {
    return true;
  } else if (n % 2 == 0) {
    return false;
  }

  // test all odd number from 3 to n / 2
  // to see if they are a factor of n
  for (int i = 3; i <= (n / 2); i += 2) {
    if (n % i == 0) {
      return false;
    }
  }

  return true;
}
