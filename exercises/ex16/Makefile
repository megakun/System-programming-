# ex16
# Haokun Cai
# Haokun@uw.edu
# 1832787
# 08/10/20
# Copyright[2019]<Haokun Cai>

# default target
ex16: ex16.o listen.o
	g++ -Wall -std=c++11 -g -o ex16 ex16.o listen.o

# individual source files
listen.o: listen.cc listen.h
	g++ -Wall -std=c++11 -g -c listen.cc

ex16.o: ex16.cc listen.h
	g++ -Wall -std=c++11 -g -c ex16.cc

# phony target
clean:
	rm -rf ex16 *.o *~ *.dSYM
	