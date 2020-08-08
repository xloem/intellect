LINK.o=$(CXX) -Wl,-rpath,$$(pwd)
CXXFLAGS=-std=c++1z -ggdb -Wall -Werror
#-O0 -static-libstdc++ -static-libgcc -static

all:
	for a in $$(ls *pp | grep -v '-'); do ln -f $$(ls "$${a%.*}"-*."$${a#*.}" | tail -n -1) "$$a"; done
	make main && ./main

main: main.o 
main.o: main.cpp *.hpp
