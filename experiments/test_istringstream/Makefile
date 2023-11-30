CXX = g++
CXXFLAGS = -std=c++14 -Wall -pipe -O2 -pedantic-errors -g

output: test_istringstream.o
	${CXX} ${CXXFLAGS} -o output test_istringstream.o

test_istringstream.o: test_istringstream.cpp
	${CXX} -c -g test_istringstream.cpp

clean:
	rm -f *.o output
