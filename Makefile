CXX=g++

CXXFLAGS=-std=c++11 -g

mizzo : A05.o candy.o
	$(CXX) $(CXXFLAGS) -o mizzo $^

A05.o : candy.h A05.C

candy.o : candy.h candy.C

clean :
	rm *.o