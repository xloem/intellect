LINK.o=${CXX}
CXXFLAGS=-std=c++1z -ggdb -Wall -Werror -I../include
LDLIBS=-L../lib -llibrary
BINFMT=$(shell lsb_release -si)-$(shell lsb_release -sr)-$(shell uname -m)
PREPROCESS=preprocess-$(BINFMT)

next: $(PREPROCESS)

test: main
	./main

main: main.o reference.o

$(PREPROCESS): preprocess.stable.cxx reference.stable.cxx reference.stable.hxx multi-any.stable.hxx
	${CXX} ${CXXFLAGS} ${CPPFLAGS} preprocess.stable.cxx reference.stable.cxx ${LDLIBS} -o $@

clean:
	-rm main *.o *.*xx

.PRECIOUS: multi-any.stable.hxx reference.stable.cxx reference.stable.cxx preprocess.stable.hxx preprocess.stable.cxx
.SUFFIXES:

%: %.o
	${LINK.o} ${CXXFLAGS} $^ ${LDLIBS} -o $@
%.o: %.cxx $(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c $< -o $@

%.stable.cxx: %.cxx $(PREPROCESS) test
	sed -e 's/^\(#include .*\)\.cxx"/\1.stable.cxx"/' $< > $@
%.stable.hxx: %.hxx $(PREPROCESS) test
	sed -e 's/^\(#include .*\)\.hxx"/\1.stable.hxx"/' $< > $@
%.cxx: %.cpp preprocess.bash #$(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	./$(PREPROCESS) $< $@
%.hxx: %.hpp preprocess.bash
	./$(PREPROCESS) $< $@
