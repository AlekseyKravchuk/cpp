CXX = g++
CXXFLAGS = -std=c++11 -Wall -pipe -O2 -pedantic-errors -g

bheap_from_array: bh_from_array.o
	${CXX} ${CXXFLAGS} -o bheap_from_array bh_from_array.o

bh_from_array.o: bh_from_array.cpp
	${CXX} -c -g bh_from_array.cpp

clean:
	rm -f *.o
	rm -f bheap_from_array
