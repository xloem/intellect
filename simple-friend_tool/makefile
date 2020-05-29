CXXFLAGS=-std=c++1z

test: simple
	./test.bash

all: simple

.SUFFIXES:

%: %.o
	g++ ${CXXFLAGS} ${LDFLAGS} $^ -o $@

%.o: %.cxx
	g++ ${CXXFLAGS} ${CPPFLAGS} -c $^ -o $@

%.cxx: %.cpp preprocess.bash *.h*
	./preprocess.bash $< $@
