// ex14
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/30/20
// Copyright[2019]<Haokun Cai>

#include<math.h>
#include <string>

#include "./Cartesian.h"

using namespace std;

Cartesian::Cartesian(float x, float y) {
    x_ = x;
    y_ = y;
}

float Cartesian::Distance(const Cartesian &other) {
    float x1 = x_;
    float y1 = y_;
    float x2 = other.x_;
    float y2 = other.y_;

    float res = (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1);
    return sqrt(res);
}

string Cartesian::ToString() const {
    string point;
    point = "(" + to_string(x_) + ", " + to_string(y_) + ")";
    return point;
}

