// ex14
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/30/20
// Copyright[2019]<Haokun Cai>

#ifndef _POLAR_H_
#define _POLAR_H_

#include<string>

#include "./Coordinate.h"

using namespace std;

class Polar : public Coordinate {
 public:
  Polar(float radius, float angle);

  // return distance of two objects
  float Distance(const Polar &point);

  // return a string represtation of the object
  virtual string ToString() const;

  virtual ~Polar() {}

 private:
  float radius_, angle_;
};

#endif  // _POLAR_H_
