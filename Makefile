CXX=g++

CXXFLAGS=-std=c++11 -g

mizzo : A05.o candy.o
	$(CXX) $(CXXFLAGS) -o mizzo $^

A05.o : mizzo.h A05.cpp

candy.o : mizzo.h candy.cpp

clean :
	rm *.o