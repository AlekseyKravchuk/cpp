CXX = g++
# CXXFLAGS = -std=c++11 -Wall -pipe -O2 -pedantic-errors -g
CXXFLAGS = -std=c++11 -Wall -pipe -pg -pedantic-errors -g
bin_search: bin_search.o
	${CXX} ${CXXFLAGS} -o bin_search bin_search.o

bin_search.o: bin_search.cpp
	${CXX} -c -pg -g bin_search.cpp

clean:
	rm -f *.o
	rm -f bin_search
