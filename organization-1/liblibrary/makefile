CXX=test/measure g++
#CXX=g++
LINK.o=${CXX}
CXXFLAGS=-time -std=c++17 -ggdb -Wall -Werror -O0
CPPFLAGS=-I. -MMD -I/usr/include/trilinos -DFBWIDTH=$$(cut -d ',' -f 1 /sys/class/graphics/fb0/virtual_size) -DFBHEIGHT=$$(cut -d ',' -f 2 /sys/class/graphics/fb0/virtual_size)
LDLIBS=-Lsource -llibrary
PREFIX=/usr/local

SHELL=bash

include $(wildcard optional.*.mk)

all: tests
tests: test/testtime-run test/testtype-run test/teststring-run test/testvectors-run test/testframebuffer-basic-run test/testframebuffer-kokkos-run test/testframebuffer-fastor-run $(patsubst %, test/test%-run, $(OPTIONALS))

clean:
	-rm */*.a */*.o

%-run: % source/liblibrary.a
	./$*

# source/regular_expression.o
source/liblibrary.a: source/string.o source/type.o source/any.o source/assert.o source/time.o source/framebuffer-basic.o source/framebuffer-kokkos.o source/heapvector.o source/stackvector.o source/unix_socket.o $(patsubst %, source/%.o, $(OPTIONALS))
	ar cr "$@" $^

scratch/demo: scratch/demo.o source/liblibrary.a
#
test/testtype: test/testtype.o source/liblibrary.a
test/teststring: test/teststring.o source/liblibrary.a
test/testvectors: test/testvectors.o source/liblibrary.a
test/testtime: test/testtime.o source/liblibrary.a
test/testany: test/testany.o source/liblibrary.a
test/testcriu: test/testcriu.o source/liblibrary.a
test/testframebuffer-basic: test/testframebuffer-basic.o source/liblibrary.a
test/testframebuffer-armadillo: test/testframebuffer-armadillo.o source/liblibrary.a
test/testframebuffer-kokkos: test/testframebuffer-kokkos.o source/liblibrary.a -ltrilinos_kokkoscore
test/testframebuffer-fastor: test/testframebuffer-fastor.o source/liblibrary.a

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
