CXX=g++

CXXFLAGS=-std=c++11 -g

pagetable : PageTable.o byu_tracereader.o
	$(CXX) $(CXXFLAGS) -o pagetable $^

PageTable.o : PageInit.h PageTable.C

byu_tracereader.o: PageInit.h byu_tracereader.C

clean :
	rm *.o