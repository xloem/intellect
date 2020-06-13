LINK.o=${CXX}
CXXFLAGS=-std=c++17 -ggdb -Wall -Werror -O3
CPPFLAGS=-I.
LDLIBS=-Lsource -llibrary
PREFIX=/usr/local

all: test/test-run

clean:
	-rm */*.a */*.o

%-run: source/liblibrary.a %
	./$*

scratch/demo: scratch/demo.o source/liblibrary.a
test/test: test/test.o source/liblibrary.a

source/liblibrary.a: source/quick.o source/framebuffer.o
	ar cr "$@" $^

install: run_test source/liblibrary.a library/*.hpp
	mkdir -p ${PREFIX}/lib
	cp source/liblibrary.a ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/library
	cp library/*.hpp ${PREFIX}/include/library

uninstall:
	-rm -rf ${PREFIX}/lib/liblibrary.a
	-rm -rf ${PREFIX}/include/library
