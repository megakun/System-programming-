// ex8
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/13/20
// Copyright[2019]<Haokun Cai>

#include <inttypes.h>
#include <cstdlib>
#include <iostream>

using namespace std;

bool isfactor(uint64_t num, uint64_t factor) {
    if (factor > num) {
        return false;
    }

    return ( num % factor == 0);
}

int main(int argc, char** argv) {
    uint64_t num;

    cout << "Which positive integer would you like me to factorize?";
    cin >> num;

    if (num <= 0) {
        cout << "Number is not valid." << endl;
        return EXIT_FAILURE;
    }

    for (uint64_t i = 1; i <= num; i++) {
        if (isfactor(num, i)) {
            cout << " " << i;
        }
    }

    cout << endl;
    return EXIT_SUCCESS;
}
