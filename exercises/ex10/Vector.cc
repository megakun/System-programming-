// ex1-
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/17/20
// Copyright[2019]<Haokun Cai>

#include <iostream>

#include "Vector.h"

float get_x();
float get_y();
float get_z();


Vector &Vector::operator=(const Vector &rhs) {
  if (this != &rhs) {
    x_ = rhs.x_;
    y_ = rhs.y_;
    z_ = rhs.z_;
  }
  return *this;
}

Vector Vector::operator+=(const Vector &v) {
  return Vector(x_ + v.x_, y_ + v.y_, z_ + v.z_);
}

Vector Vector::operator-=(const Vector &v) {
  return Vector(x_ - v.x_, y_ - v.y_, z_ - v.z_);
}

float Vector::operator*(const Vector &v) {
  return x_*v.x_ + y_*v.y_ + z_*v.z_;
}
