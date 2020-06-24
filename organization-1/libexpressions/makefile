LINK.o=g++
CXXFLAGS=-std=c++17 -ggdb
CPPFLAGS=-I../liblibrary -I.
LDLIBS=-L../liblibrary/source -Lsource -llibrary -lexpressions
CXX=./measure g++

all: test/test-run
clean:
	-rm */*.o */test

%-run: source/libexpressions.a %
	./$*
source/libexpressions.a: source/syntax_operator.o
	ar cr "$@" $^

%.E: %.cpp
	$(CXX) -E $(CXXFLAGS) $(CPPFLAGS) $^ -o $@

test/test: test/test.o

test/test.o: expressions/*.hpp
