// ex9
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/15/20
// Copyright[2019]<Haokun Cai>

#include <cstdlib>
#include <iostream>

#include "IntPair.h"

void test(IntPair pair);

int main(int argc, char **argv) {
    IntPair p(0, 1);
    test(p);

    int xvalue, yvalue;
    p.get(&xvalue, &yvalue);

    if  ((xvalue != 0) || (yvalue != 1)) {
        std::cout << "Is pass by reference" << std::endl;
    } else {
        std::cout << "Is pass by value"  << std::endl;
    }
    return EXIT_SUCCESS;
}

void test(IntPair pair) {
    int x, y;
    pair.get(&x, &y);
    pair.set(x+1, y+1);
}
