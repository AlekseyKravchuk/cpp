CXX = g++
CXXFLAGS = -std=c++11 -Wall -pipe -O2 -pedantic-errors -g

heapsort: heapsort.o
	${CXX} ${CXXFLAGS} -o heapsort heapsort.o

heapsort.o: heapsort.cpp
	${CXX} -c -g heapsort.cpp

clean:
	rm -f *.o
	rm -f heapsort
