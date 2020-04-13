CXX=g++

CXXFLAGS=-std=c++11 -g

mizzo : producer.o consumer.o belt.o
	$(CXX) $(CXXFLAGS) -o pagetable $^

producer.o : mizzo.h producer.cpp

consumer.o : mizzo.h consumer.cpp

belt.o : mizzo.h belt.cpp

clean :
	rm *.o