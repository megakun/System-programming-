// ex12a
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/28/20
// Copyright[2019]<Haokun Cai>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <map>

using namespace std;

int main(int argc, char **argv) {
    map<string, int> record;
    ifstream input;
    string word;
    string filename;

    //  takes file name in the same line command
    filename = argv[1];

    input.open(filename.c_str());
    if (!input) {
        cout << "unable to open file" << endl;
       return EXIT_FAILURE;
    }

    while (input >> word) {
        if (record.count(word)) {
            record[word] += 1;
        } else {
            record[word] = 1;
        }
    }

    for (auto elem : record) {  // prints out all the sorted key and values
        cout << elem.first << " " << elem.second << "\n";
    }

    return EXIT_SUCCESS;
}
