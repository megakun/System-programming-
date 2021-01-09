// ex3
// Haokun Cai
// haokun@uw.edu
// 1832787
// 06/29/19
// Copyright[2019]<Haokun Cai>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
// struct for x y z in a point3d
typedef struct point3d {
  int32_t x, y, z;
} Point3d;

// helper method
Point3d *AllocatePoint3d(int32_t x, int32_t y, int32_t z);

int main(int argc, char **argv) {
  Point3d *pt = AllocatePoint3d(1, 2, 3);
  // check if anything is actually allocated
  if (pt == NULL) {
    printf("AllocatePoint3d() failed.\n");
    return EXIT_FAILURE;
  }
  // check if correct value if allocated
  if (pt -> x != 1 || pt -> y != 2 || pt -> z != 3) {
    printf("one or more points contain wrong value\n");
    return EXIT_FAILURE;
  }

  printf("everthing works, yay.\n");
  // free memory allocated
  free(pt);
  return EXIT_SUCCESS;
}

Point3d *AllocatePoint3d(int32_t x, int32_t y, int32_t z) {
  Point3d *ptr = (Point3d *)malloc(sizeof(Point3d));  // allocate memory
  // asign values
  ptr->x = x;
  ptr->y = y;
  ptr->z = z;

  return ptr;
}
