CXX=test/measure g++
#CXX=g++
LINK.o=${CXX}
CXXFLAGS=-time -std=c++17 -ggdb -Wall -Werror -O0
CPPFLAGS=-I.
LDLIBS=-Lsource -llibrary
PREFIX=/usr/local

SHELL=bash

all: tests
tests: test/teststring-run test/testvectors-run test/test-run

clean: depend
	-rm */*.a */*.o

%-run: source/liblibrary.a %
	./$*

source/liblibrary.a: source/string.o source/quick.o source/framebuffer.o source/assert.o source/regular_expression.o
	ar cr "$@" $^

scratch/demo: scratch/demo.o source/liblibrary.a

test/teststring: test/teststring.o source/liblibrary.a
test/testvectors: test/testvectors.o source/liblibrary.a
test/test: test/test.o source/liblibrary.a

depend:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $(wildcard */*.cpp) > .depend
include .depend

%.E: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $< -o $@
%.o: %.cpp 
	set -o pipefail; $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@ 2>&1| head -n 40

install: run_test source/liblibrary.a library/*.hpp
	mkdir -p ${PREFIX}/lib
	cp source/liblibrary.a ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/library
	cp library/*.hpp ${PREFIX}/include/library

uninstall:
	-rm -rf ${PREFIX}/lib/liblibrary.a
	-rm -rf ${PREFIX}/include/library
