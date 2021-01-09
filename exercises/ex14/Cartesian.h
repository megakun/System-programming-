// ex14
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/30/20
// Copyright[2019]<Haokun Cai>

#ifndef _CARTESIAN_H_
#define _CARTESIAN_H_

#include <string>

#include "./Coordinate.h"

using namespace std;

class Cartesian : public Coordinate {
 public:
  Cartesian(float x, float y);

  //  return distance of two objects
  float Distance(const Cartesian &point);

  //  return a string represtation of the object
  virtual string ToString() const;

  virtual ~Cartesian() {}

 private:
  float x_, y_;
};

#endif  // _CARTESIAN_H_
