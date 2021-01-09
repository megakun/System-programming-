// ex10
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/17/20
// Copyright[2019]<Haokun Cai>

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

class Vector {
 public:
    Vector() {
        x_ = 0.0;
        y_ = 0.0;
        z_ = 0.0;
    }

    Vector(const float x, const float y, const float z) {
        x_ = x;
        y_ = y;
        z_ = z;
    }

    Vector(const Vector &v) {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
    }

    ~Vector() {}

    float get_x() const {
        return x_;
    }
    float get_y() const {
        return y_;
    }
    float get_z() const {
        return z_;
    }

    Vector &operator=(const Vector &v);
    Vector operator+=(const Vector &other);
    Vector operator-=(const Vector &other);
    float operator*(const Vector &other);

 private:
    float x_, y_, z_;
};

#endif  // _VECTOR_H_
