CXX = g++
CXXFLAGS = -std=c++17 -O2 -fopenmp

all: main

main: main.o quicksort.o mergesort.o utilities.o
	$(CXX) $(CXXFLAGS) -o main main.o quicksort.o mergesort.o utilities.o

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

quicksort.o: quicksort.cpp quicksort.hpp
	$(CXX) $(CXXFLAGS) -c quicksort.cpp

mergesort.o: mergesort.cpp mergesort.hpp
	$(CXX) $(CXXFLAGS) -c mergesort.cpp

utilities.o: utilities.cpp utilities.hpp
	$(CXX) $(CXXFLAGS) -c utilities.cpp

clean:
	rm -f *.o main
