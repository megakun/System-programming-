// ex9
// Haokun Cai
// Haokun@uw.edu
// 1832787
// 07/15/20
// Copyright[2019]<Haokun Cai>

CC = g++
CFLAGS = -Wall -std=c++11 -g

ex9: ex9.o IntPair.o
	$(CC) $(CFLAGS) -o ex9 ex9.o IntPair.o

ex9.o: ex9.cc IntPair.h
	$(CC) $(CFLAGS) -c ex9.cc

IntPair.o: IntPair.cc IntPair.h
	$(CC) $(CFLAGS) -c IntPair.cc

clean:
	rm ex9 *.o