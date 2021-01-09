// ex14
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/30/20
// Copyright[2019]<Haokun Cai>

#ifndef _COORDINATE_H_
#define _COORDINATE_H_

#include <string>

using namespace std;

class Coordinate {
 public:
  virtual string ToString() const = 0;  // pure virtual method
};

#endif  // _COORDINATE_H_
