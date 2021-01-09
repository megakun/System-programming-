//ex0.c
//Haokun Cai
//haokun@uw.edu
//1832787
//06/23/19

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Finding the nth term in the series
double NthTerm(uint64_t n){
  double term = 4.0;
  term = term / (2.0 * n);
  term = term / (2.0 * n + 1);
  term = term / (2.0 * n + 2);
  if (n % 2 == 0){
    term = term * -1.0;
  }
  return term;
}

//Return the estimate
int main (int argc, char **argv) {
  uint64_t res;
  double base = 3.0;//first num of the series
  
  //if arguments number does not fit
  if(argc != 2) {
    printf("ERROR: This program takes only one non-negative integer as argument.\n");
    exit(EXIT_FAILURE);
  }
  
  //try to convert noninteger to integer, otherwise error
  if(sscanf(argv[1], "%lld", (long long int *)&res) != 1) {
    printf ("ERROR: %s is not an positive integer.\n", argv[1]);
    exit(EXIT_FAILURE);
  } else if(argv[1] < 0) {
    printf ("ERROR: %s is not an positive integer.\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  for(uint64_t i = 1; i <= res; i++){
    base += NthTerm(i);
  }

  printf("Our estimate of Pi is %.20f\n", base);

  return EXIT_SUCCESS;
}



