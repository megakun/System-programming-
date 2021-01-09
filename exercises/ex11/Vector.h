// ex11
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/20/20
// Copyright[2019]<Haokun Cai>

#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <iostream>

namespace vector333 {

class Vector {
 public:
    Vector() {
        arr_ = new float[3]();
    }

    Vector(const float x, const float y, const float z) {
        arr_ = new float[3]{x, y, z};
    }

    Vector(const Vector &v) {
        arr_ = new float[3]{v.arr_[0], v.arr_[1], v.arr_[2]};
    }

    ~Vector() {
        delete[] arr_;
    }

    Vector &operator=(const Vector &v);
    Vector operator+=(const Vector &other);
    Vector operator-=(const Vector &other);
    Vector operator*(const float a);
    float operator*(const Vector &other);

    friend std::ostream &operator<<(std::ostream &out, const Vector &v);
    friend Vector operator+(const Vector &a, const Vector &b);
    friend Vector operator-(const Vector &a, const Vector &b);

 private:
    float *arr_;
};

}  // namespace vector333

#endif  // _VECTOR_H_
