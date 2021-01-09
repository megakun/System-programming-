// ex9
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/15/20
// Copyright[2019]<Haokun Cai>

#ifndef _INTPAIR_H_
#define _INTPAIR_H_

class IntPair {
 public:
    IntPair(int x, int y);

    void get(int* x, int* y);

    void set(int x, int y);

 private:
    int x_;
    int y_;
};

#endif  // _INTPAIR_H_
