// ex11
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/20/20
// Copyright[2019]<Haokun Cai>

#include <iostream>
#include "Vector.h"

using namespace std;
using namespace vector333;



int main(int argc, char **argv) {
    // test constructors
    Vector v1;
    cout << "default constructor test" << endl;
    cout << "expect: (0,0,0)" << endl;
    cout << "actual: " << v1 << endl;

    Vector v2(1.0, -2.0, 3.0);
    cout << "Initializing constructor test" << endl;
    cout << "expect: (1,-2,3)" << endl;
    cout << "actual: " << v2 << endl;

    Vector v3(v2);
    cout << "copy constructor test" << endl;
    cout << "expect: (1,-2,3)" << endl;
    cout << "actual: " << v3 << endl;

    // test assigment
    Vector v4;
    v4 = v2;
    cout << "assignment test" << endl;
    cout << "expect: (1,-2,3)" << endl;
    cout << "actual: " << v4 << endl;


    // test arithmetics
    Vector v5(1.0, 1.0, 1.0);
    v5 += v3;
    cout << "+= test" << endl;
    cout << "expect: (2,-1,4)" << endl;
    cout << "actual: " << v5 << endl;

    v5 -= v3;
    cout << "-= test" << endl;
    cout << "expect: (1,1,1)" << endl;
    cout << "actual: " << v5 << endl;

    float res = v2 * v3;
    cout << "* test 1" << endl;
    cout << "expect: 14" << endl;
    cout << "actual: " << res << "\n" << endl;

    Vector v6 = v5 * 4;
    cout << "* test 2" << endl;
    cout << "expect: (4,4,4)" << endl;
    cout << "actual: " << v6 << endl;

    cout << "nice! everything works!" << endl;
    return EXIT_SUCCESS;
}
