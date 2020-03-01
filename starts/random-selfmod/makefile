CXX=g++-8
CXXFLAGS=-Wall -Werror -std=gnu++17 -fno-operator-names -ggdb -O3 -pthread -fno-toplevel-reorder
LDLIBS=-lcapstone
LINK.o=$(LINK.cc) -v
go: 03-self-improve-form
	./$^
03-self-improve-form: 03-self-improve-form.o
intellect: intellect.cpp
	g++ -ggdb -std=c++11 intellect.cpp  -Wl,--print-map
