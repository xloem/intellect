LINK.o=$(CXX) -Wl,-rpath,$$(pwd)
CXXFLAGS=-std=c++1z -ggdb -Wall -Werror 

all:
	for a in $$(ls *pp | grep -v '-'); do ln -f $$(ls "$${a%.*}"-* | tail -n -1) "$$a"; done
	make main && ./main

main: main.o
main.o: main.cpp *.hpp
