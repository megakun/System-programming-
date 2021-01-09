// ex12
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/25/20
// Copyright[2019]<Haokun Cai>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

// read in values from input stream
// return a float  value if successful
// otherwise error
template <typename T> T ReadValue(istream& in);

int main(int argc, char **argv) {
    vector<float> v;

    cout << "Input 8 floats:" << endl;

    for (int i = 0; i < 8; i++) {
        v.push_back(ReadValue<float>(cin));
    }
    sort(v.begin(), v.end());
    cout << "Your sorted floats are:" << endl;
    for (auto& p : v) {
      cout << p << endl;
    }

    return EXIT_SUCCESS;
}

template <typename T> T ReadValue(istream& in) {
  T value;
  in >> value;

  if (in.eof()) {
    cout << "EOF failure." << endl;
    exit(EXIT_FAILURE);
  } else if (in.fail()) {
    cout << "invalid inputs." << endl;
    exit(EXIT_FAILURE);
  }
  return value;
}




