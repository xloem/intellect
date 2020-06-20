LINK.o=g++
CXXFLAGS=-std=c++17 -ggdb
CPPFLAGS=-I../liblibrary -I.
LDLIBS=-L../liblibrary/source -llibrary
CXX=./measure g++

all: test/test-run
clean:
	-rm */*.o */test

%-run: %
	./$*

test/test: test/test.o

test/test.o: expressions/*.hpp
