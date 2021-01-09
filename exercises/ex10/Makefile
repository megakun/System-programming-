# ex10
# Haokun Cai
# Haokun@uw.edu
# 1832787
# 07/17/20
# Copyright[2019]<Haokun Cai>

CC = g++
CFLAGS = -Wall -std=c++11 -g

ex10: ex10.o Vector.o
	$(CC) $(CFLAGS) -o ex10 ex10.o Vector.o

ex10.o: ex10.cc Vector.h
	$(CC) $(CFLAGS) -c ex10.cc

Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c Vector.cc

clean: 
	rm ex10 *.o