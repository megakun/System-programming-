// ex14
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/30/20
// Copyright[2019]<Haokun Cai>

#include <math.h>
#include <string>

#include "./Polar.h"

using namespace std;

Polar::Polar(float radius, float angle) {
  radius_ = radius;
  angle_ = angle;  // in degree
}

float Polar::Distance(const Polar &other) {
  float r1 = radius_;
  float a1 = angle_;
  float r2 = other.radius_;
  float a2 = other.angle_;

  // use Polar coordinate distance equation:
  float tmp = (r1*r1) + (r2*r2) - (2*r1*r2)*cos(M_PI * (a1-a2)/180);

  return sqrt(tmp);
}

string Polar::ToString() const {
    string point;
    point = "(" + to_string(radius_) + ", " + to_string(angle_) + ")";
    return point;
}
