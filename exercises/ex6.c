// ex6
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/07/20
// Copyright[2019]<Haokun Cai>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("wrong arugment number\n");
    return EXIT_FAILURE;
  }

  FILE *file = fopen(argv[1], "rb");
  if (file == NULL) {
    printf("no such file\n");
    return EXIT_FAILURE;
  }

  if (fseek(file, 0, SEEK_END) != 0) {
    printf("fseek failed\n");
    return EXIT_FAILURE;
  }

  int fsize;
  fsize = ftell(file);
  if (fsize == -1) {
    printf("wrong file size\n");
    return EXIT_FAILURE;
  }
  // print the file backwards
  for (int i = fsize - 1; i >= 0; i--) {
    char c;
    fseek(file, i, SEEK_SET);
    fread((char*)&c, sizeof(char), 1, file);
    printf("%c", c);
  }
  fclose(file);
  return EXIT_SUCCESS;
}

