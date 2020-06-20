LINK.o=${CXX}
CXXFLAGS=-std=c++1z -ggdb -Wall -Werror -I../include
LDLIBS=-L../lib -llibrary

test: main
	./main

main: main.o reference.o
preprocess: preprocess.o reference.o

clean:
	-rm main *.o *.*xx

.PRECIOUS: multi-any.hxx reference.cxx reference.hxx preprocess.cxx preprocess.hxx
.SUFFIXES:

%: %.o
	${LINK.o} ${CXXFLAGS} $^ ${LDLIBS} -o $@
%.o: %.cxx
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c $^ -o $@

%.cxx: %.cpp ./preprocess preprocess.bash
	./preprocess $< $@
%.hxx: %.hpp ./preprocess preprocess.bash
	./preprocess $< $@
