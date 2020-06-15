LINK.o=g++
CXXFLAGS=-std=c++17 -ggdb
CPPFLAGS=-I../liblibrary
LDLIBS=-L../liblibrary/source -llibrary
CXX=./measure g++

all: expressions/test-run
clean:
	-rm */*.o */test

%-run: %
	./$*

expressions/test: expressions/test.o

expressions/test.o: expressions/*.hpp
