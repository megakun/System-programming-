// ex13
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 08/02/20
// Copyright[2019]<Haokun Cai>

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>

// already replace all the ptr with unique ptr to smart delete
int main(int argc, char **argv) {
  // Allocate an integer on the heap, initialize to value 5.
  std::unique_ptr<int> x(new int(5));
  std::cout << "*x is: " << *x << std::endl;

  // Allocate a vector of integers on the heap, add some values to
  // that vector, sort the vector, print the values.
  std::unique_ptr<std::vector<int>> v(new std::vector<int>);
  v->push_back(13);
  v->push_back(42);
  v->push_back(17);
  std::sort(v->begin(), v->end());
  std::cout << "sorted v: ";
  for (int &el : *v) {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  // Allocate a vector of (integer pointers) on the stack, add some
  // values to the vector from the heap, print the values.
  std::vector<std::unique_ptr<int>> v2;
  v2.push_back(std::unique_ptr<int>(new int (13)));
  v2.push_back(std::unique_ptr<int>(new int(42)));
  v2.push_back(std::unique_ptr<int>(new int(17)));
  std::cout << "unsorted v2: ";
  for (auto &el : v2) {
    std::cout << *el << " ";
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
