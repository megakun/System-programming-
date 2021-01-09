# ex11
# Haokun Cai
# Haokun@uw.edu
# 1832787
# 07/20/20
# Copyright[2019]<Haokun Cai>

CC = g++
CFLAGS = -Wall -std=c++11 -g

ex11: ex11.o Vector.o
	$(CC) $(CFLAGS) -o ex11 ex11.o Vector.o

ex11.o: ex11.cc Vector.h
	$(CC) $(CFLAGS) -c ex11.cc

Vector.o: Vector.cc Vector.h
	$(CC) $(CFLAGS) -c Vector.cc

clean: 
	rm ex11 *.o