CXXFLAGS=-std=c++1z -ggdb -Wall -Werror

test: main
	./main

main: main.o reference.o

.SUFFIXES:

%: %.o
	${CXX} ${CXXFLAGS} ${LDFLAGS} $^ -o $@
%.o: %.cxx
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c $^ -o $@

%.cxx: %.cpp preprocess.bash $(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	bash preprocess.bash $< $@
%.hxx: %.hpp preprocess.bash
	bash preprocess.bash $< $@
