CXX = g++
CXXFLAGS = -std=c++11 -Wall -pipe -O2 -pedantic-errors -g

bfs: bfs.o
	${CXX} ${CXXFLAGS} -o bfs bfs.o

bfs.o: bfs.cpp
	${CXX} -c -g bfs.cpp

clean:
	rm -f *.o
	rm bfs
