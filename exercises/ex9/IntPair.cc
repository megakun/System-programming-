// ex9
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/15/20
// Copyright[2019]<Haokun Cai>

#include <iostream>

#include "IntPair.h"

IntPair::IntPair(int x, int y) {
  x_ = x;
  y_ = y;
}

void IntPair::get(int *x, int *y) {
  *x = x_;
  *y = y_;
}

void IntPair::set(int x, int y) {
  x_ = x;
  y_ = y;
}
