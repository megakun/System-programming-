// ex14
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/30/20
// Copyright[2019]<Haokun Cai>

#include <iostream>

#include "./Cartesian.h"
#include "./Polar.h"

using namespace std;

int main() {
    float dist;

    // test for Cartesian
    // test constructor
    Cartesian* c1 = new Cartesian(2.1, 5.2);
    Cartesian* c2 = new Cartesian(4, 3.89);

    cout << "Cartesian Verification" << endl;
    cout << "expect: (2.1, 5.2) actual: " << c1->ToString()  << endl;
    cout << "expect: (4, 3.89) actual: " << c2->ToString()  << endl;

    //  test distance
    dist = c1->Distance(*c2);
    cout << "expect: 2.30783 actual: " << dist << endl;

    // test for polar
    // test constructor
    Polar* p1 = new Polar(1, 90);
    Polar* p2 = new Polar(2, 45);

    cout << "Polar Verification" << endl;
    cout << "expect: (1, 90) actual: " << p1->ToString()  << endl;
    cout << "expect: (2, 45) actual: " << p2->ToString()  << endl;

    dist = p1->Distance(*p2);
    cout << "expect: 1.47363 actual: " << dist << endl;

    delete c1;
    delete c2;
    delete p1;
    delete p2;

    cout << "Everthing is fine!" << endl;

    return EXIT_SUCCESS;
}
