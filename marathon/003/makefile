LINK.o=$(CXX)
CXXFLAGS=-std=c++1z -ggdb -Wall -Werror

all:
	FILE=$$(ls *.cpp | tail -n 1 | sed 's/\.cpp//'); rm "$$FILE"; make "$$FILE" && ./$$FILE
%-run: %
	./$^
