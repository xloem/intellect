CXXFLAGS=-std=c++1z -ggdb -Wall -Werror

test: simple
	./test.bash

all: simple

.SUFFIXES:

%: %.o makefile
	g++ ${CXXFLAGS} ${LDFLAGS} $< -o $@

%.o: %.cxx
	g++ ${CXXFLAGS} ${CPPFLAGS} -c $^ -o $@

%.cxx: %.cpp preprocess.bash *.h*
	./preprocess.bash $< $@
