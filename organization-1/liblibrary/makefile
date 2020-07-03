CXX=test/measure g++
#CXX=g++
LINK.o=${CXX}
CXXFLAGS=-time -std=c++17 -ggdb -Wall -Werror -O0
CPPFLAGS=-I. -MMD
LDLIBS=-Lsource -llibrary
PREFIX=/usr/local

SHELL=bash

all: tests
tests: test/testtype-run test/teststring-run test/testvectors-run test/testframebuffer-basic-run test/testframebuffer-armadillo-run

clean:
	-rm */*.a */*.o

%-run: source/liblibrary.a %
	./$*

# source/regular_expression.o
source/liblibrary.a: source/string.o source/assert.o source/framebuffer-basic.o source/framebuffer-armadillo.o source/time.o source/heapvector.cpp source/stackvector.cpp
	ar cr "$@" $^

scratch/demo: scratch/demo.o source/liblibrary.a

test/testtype: test/testtype.o source/liblibrary.a
test/teststring: test/teststring.o source/liblibrary.a
test/testvectors: test/testvectors.o source/liblibrary.a
test/testframebuffer-basic: test/testframebuffer-basic.o source/liblibrary.a
test/testframebuffer-armadillo: test/testframebuffer-armadillo.o source/liblibrary.a

include $(wildcard */*.d)

%.E: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $< -o $@
%.o: %.cpp 
	set -o pipefail; $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@ 2>&1| head -n 40

install: source/liblibrary.a library/*.hpp
	mkdir -p ${PREFIX}/lib
	cp source/liblibrary.a ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/library
	cp library/*.hpp ${PREFIX}/include/library

uninstall:
	-rm -rf ${PREFIX}/lib/liblibrary.a
	-rm -rf ${PREFIX}/include/library
