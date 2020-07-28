LINK.o=${CXX}
NODE4_DIR=$(dir $(lastword $(MAKEFILE_LIST)))
BINFMT=$(shell lsb_release -si)-$(shell lsb_release -sr)-$(shell uname -m)
PREPROCESS=${NODE4_DIR}/node4-preprocess-${BINFMT}

CXXFLAGS=-std=c++1z -ggdb -Wall -Werror

%.o: %.cxx $(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	${CXX} ${CXXFLAGS} ${CPPFLAGS} -c $< -o $@

%.cxx: %.cpp ${NODE4_DIR}/node4-preprocess.bash #$(patsubst %.hpp,%.hxx, $(wildcard *.hpp))
	./$(PREPROCESS) $< $@
%.hxx: %.hpp ${NODE4_DIR}/node4-preprocess.bash
	./$(PREPROCESS) $< $@
