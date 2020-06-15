CXX=test/measure g++
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

source/liblibrary.a: source/string.o source/quick.o source/framebuffer.o source/assert.o
	ar cr "$@" $^

scratch/demo: scratch/demo.o source/liblibrary.a
test/test: test/test.o source/liblibrary.a

%.E: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $^ -o $@
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -c -o $@ 2>&1| head -n 40

install: run_test source/liblibrary.a library/*.hpp
	mkdir -p ${PREFIX}/lib
	cp source/liblibrary.a ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/library
	cp library/*.hpp ${PREFIX}/include/library

uninstall:
	-rm -rf ${PREFIX}/lib/liblibrary.a
	-rm -rf ${PREFIX}/include/library
