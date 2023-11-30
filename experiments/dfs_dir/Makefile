# This Makefile is designed to build preorder program, which preorder traverse binary tree

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pipe -O2 -pedantic-errors -g

preorder: preorder.o
	${CXX} ${CXXFLAGS} -o preorder preorder.o

preorder.o: preorder.cpp
	${CXX} -c -g preorder.cpp

clean:
	rm -f *.o
	rm preorder
