# ex14
# Haokun Cai
# Haokun@uw.edu
# 1832787
# 07/30/20
# Copyright[2019]<Haokun Cai>

CC = g++
CFLAGS = -Wall -std=c++11 -g

ex14: ex14.o Cartesian.o Polar.o
	$(CC) $(CFLAGS) -o ex14 ex14.o Cartesian.o Polar.o

ex14.o: ex14.cc Cartesian.h Polar.h
	$(CC) $(CFLAGS) -c ex14.cc

Cartesian.o: Cartesian.cc Cartesian.h
	$(CC) $(CFLAGS) -c Cartesian.cc

Polar.o: Polar.cc Polar.h
	$(CC) $(CFLAGS) -c Polar.cc
clean: 
	rm ex14 *.o *~ *.dSYM