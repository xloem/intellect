LINK.o=${CXX}
CXXFLAGS=-std=c++17 -ggdb
CPPFLAGS=-I.
LDFLAGS=-Lsource
LDLIBS=-llibrary
PREFIX=/usr/local

run_test: test/test
	test/test

source/liblibrary.a: source/quick.o source/framebuffer.o
	ar cr "$@" $^

test/test: test/test.o source/liblibrary.a

install: run_test source/liblibrary.a library/*.hpp
	mkdir -p ${PREFIX}/lib
	cp source/liblibrary.a ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/library
	cp library/*.hpp ${PREFIX}/include/library

uninstall:
	-rm -rf ${PREFIX}/lib/liblibrary.a
	-rm -rf ${PREFIX}/include/library
