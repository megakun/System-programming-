// ex11
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/20/20
// Copyright[2019]<Haokun Cai>

#include <iostream>

#include "Vector.h"

namespace vector333 {

Vector &Vector::operator=(const Vector &rhs) {
  if (this != &rhs) {
    this->arr_[0] = rhs.arr_[0];
    this->arr_[1] = rhs.arr_[1];
    this->arr_[2] = rhs.arr_[2];
  }
  return *this;
}

Vector Vector::operator+=(const Vector &v) {
  this->arr_[0] += v.arr_[0];
  this->arr_[1] += v.arr_[1];
  this->arr_[2] += v.arr_[2];
  return *this;
}

Vector Vector::operator-=(const Vector &v) {
  this->arr_[0] -= v.arr_[0];
  this->arr_[1] -= v.arr_[1];
  this->arr_[2] -= v.arr_[2];
  return *this;
}

float Vector::operator*(const Vector &v) {
  return (this->arr_[0] * v.arr_[0]) +
         (this->arr_[1] * v.arr_[1]) +
         (this->arr_[2] * v.arr_[2]);
}

Vector Vector::operator*(const float a) {
  return Vector(this->arr_[0] * a, this->arr_[1] * a, this->arr_[2] * a);
}

std::ostream &operator<<(std::ostream &out, const Vector &v) {
  return out << "(" << v.arr_[0] << "," << v.arr_[1] << "," << v.arr_[2]
             << ")" << "\n";
}

Vector operator+(const Vector &a, const Vector &b) {
  return Vector(a.arr_[0] + b.arr_[0],
                a.arr_[1] + b.arr_[1],
                a.arr_[2] + b.arr_[2]);
}

Vector operator-(const Vector &a, const Vector &b) {
  return Vector(a.arr_[0] - b.arr_[0],
                a.arr_[1] - b.arr_[1],
                a.arr_[2] - b.arr_[2]);
}

}  // namespace vector333
