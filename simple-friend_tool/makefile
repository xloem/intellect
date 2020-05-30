CXXFLAGS=-std=c++1z -ggdb -Wall -Werror

all: simple

test: simple
	./test.bash

.SUFFIXES:

%: %.o makefile
	g++ ${CXXFLAGS} ${LDFLAGS} $< -o $@

%.o: %.cxx
	g++ ${CXXFLAGS} ${CPPFLAGS} -c $^ -o $@

%.cxx: %.cpp preprocess.bash *.h*
	./preprocess.bash $< $@
