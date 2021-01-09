# ex15
# Haokun Cai
# Haokun@uw.edu
# 1832787
# 08/10/20
# Copyright[2019]<Haokun Cai>

# default target
ex15: ex15.o network.o
	g++ -Wall -std=c++11 -g -o ex15 ex15.o network.o

# individual source files
network.o: network.cc network.h
	g++ -Wall -std=c++11 -g -c network.cc

ex15.o: ex15.cc network.h
	g++ -Wall -std=c++11 -g -c ex15.cc

# phony target
clean:
	rm -rf ex15 *.o *~ *.dSYM
	