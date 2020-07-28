LDLIBS=-L../lib -llibrary
PREFIX=/usr/local

include include.mk

CXXFLAGS+= -I../include

next: $(PREPROCESS)

install: $(patsubst %.hpp,%.hxx, $(wildcard *.hpp)) $(PREPROCESS) libnode4.a include.mk
	mkdir -p ${PREFIX}/lib
	cp -v libnode4.a  ${PREFIX}/lib
	mkdir -p ${PREFIX}/include/node4
	cp -v *.hxx *.hpp ${PREFIX}/include/node4
	mkdir -p ${PREFIX}/share/node4
	cp -v node4-preprocess.bash $(PREPROCESS) include.mk ${PREFIX}/share/node4
	mkdir -p ${PREFIX}/bin
	ln -vs ${PREFIX}/share/node4/node4-* ${PREFIX}/bin

uninstall:
	-rm -rf ${PREFIX}/lib/libnode4.a ${PREFIX}/include/node4 ${PREFIX}/share/node4 ${PREFIX}/bin/node4-*

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

%.stable.cxx: %.cxx $(PREPROCESS) test
	sed -e 's/^\(#include .*\)\.\([ch]xx\)"/\1.stable.\2"/' $< > $@
%.stable.hxx: %.hxx $(PREPROCESS) test
	sed -e 's/^\(#include .*\)\.hxx"/\1.stable.hxx"/' $< > $@
