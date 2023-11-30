# BitmapFileHeader.h  Bitmap.h  BitmapInfoHeader.h

CXX = g++
CXXFLAGS = -std=c++11 -Wall -pipe -O2 -pedantic-errors -g

# OBJECTS = main.o bmp.o

bmp: main.o bmp.o
	${CXX} ${CXXFLAGS} -o bmp bmp.o main.o

main.o: 
	${CXX} -c -g -o main.o main.cpp

bmp.o:
	${CXX} -c -g -o bmp.o Bitmap.cpp 

clean:
	rm -f *.o bmp
 
