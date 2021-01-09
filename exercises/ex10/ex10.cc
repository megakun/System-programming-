// ex10
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/17/20
// Copyright[2019]<Haokun Cai>

#include <iostream>
using namespace std;

#include "Vector.h"

int main(int argc, char **argv) {
    // test constructors
    Vector v1;
    if (v1.get_x() == 0.0 && v1.get_y() == 0.0 && v1.get_z() == 0.0) {
        cout << "defalut constructor pass" << endl;
    } else {
        return EXIT_FAILURE;
    }

    Vector v2(1.0, -2.0, 3.0);
    if (v2.get_x() == 1.0 && v2.get_y() == -2.0 && v2.get_z() == 3.0) {
        cout << "constructor with values pass" << endl;
    } else {
        return EXIT_FAILURE;
    }

    Vector v3(v2);
    if (v3.get_x() == 1.0 && v3.get_y() == -2.0 && v3.get_z() == 3.0) {
        cout << "copy constructor pass" << endl;
    } else {
        return EXIT_FAILURE;
    }

    // test assigment
    Vector v4;
    v4 = v2;
    if (v4.get_x() == 1.0 && v4.get_y() == -2.0 && v4.get_z() == 3.0) {
        cout << "assignment pass" << endl;
    } else {
        return EXIT_FAILURE;
    }

    // test arithmetics
    Vector v5 = v2 += v3;
    if (v5.get_x() == 2.0 && v5.get_y() == -4.0 && v5.get_z() == 6.0) {
        cout << "+ pass" << endl;
    } else {
        return EXIT_FAILURE;
    }
    Vector v6 = v5 -= v2;
    if (v6.get_x() == 1.0 && v6.get_y() == -2.0 && v6.get_z() == 3.0) {
        cout << "- pass" << endl;
    } else {
        return EXIT_FAILURE;
    }
    if (v2 * v3 == 14.0 && v1 * v2 == 0.0) {
        cout << "* pass" << endl;
    } else {
        return EXIT_FAILURE;
    }
    cout << "nice! everything works!" << endl;
    return EXIT_SUCCESS;
}
