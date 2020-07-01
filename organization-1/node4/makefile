LINK.o=${CXX}
CXXFLAGS=-std=c++1z -ggdb -Wall -Werror -I../include
LDLIBS=-L../lib -llibrary
BINFMT=$(shell lsb_release -si)-$(shell lsb_release -sr)-$(shell uname -m)
PREPROCESS=preprocess-$(BINFMT)
PREFIX=/usr/local

next: $(PREPROCESS)

install: $(patsubst %.hpp,%.hxx, $(wildcard *.hpp)) $(PREPROCESS) libnode4.a
	mkdir -p ${PREFIX}/lib
	cp libnode4.a  ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/node4
	cp -va *.hxx *.hpp ${PREFIX}/include/node4
	mkdir -p ${PREFIX}/bin
	cp preprocess.bash ${PREFIX}/bin/node4-preprocess
	chmod 755 ${PREFIX}/bin/node4-preprocess

uninstall:
	-rm -rf ${PREFIX}/lib/libnode4.a ${PREFIX}/include/node4 ${PREFIX}/bin/node4-preprocess

libnode4.a: reference.o easy.o
	ar cr "$@" $^

test: main
	./main

main: main.o reference.o

$(PREPROCESS): preprocess.stable.cxx reference.stable.cxx reference.stable.hxx multi-any.stable.hxx
	${CXX} ${CXXFLAGS} ${CPPFLAGS} preprocess.stable.cxx reference.stable.cxx ${LDLIBS} -o $@

clean:
	-rm main *.o *.*xx

.PRECIOUS: multi-any.stable.hxx reference.stable.hxx reference.stable.cxx preprocess.stable.hxx preprocess.stable.cxx
.SUFFIXES:

%: %.o
	${LINK.o} ${CXXFLAGS} $^ ${LDLIBS} -o $@
%.o: %.cxx $(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c $< -o $@

%.stable.cxx: %.cxx $(PREPROCESS) test
	sed -e 's/^\(#include .*\)\.\([ch]xx\)"/\1.stable.\2"/' $< > $@
%.stable.hxx: %.hxx $(PREPROCESS) test
	sed -e 's/^\(#include .*\)\.hxx"/\1.stable.hxx"/' $< > $@
%.cxx: %.cpp preprocess.bash #$(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	./$(PREPROCESS) $< $@
%.hxx: %.hpp preprocess.bash
	./$(PREPROCESS) $< $@
