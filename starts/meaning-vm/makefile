CXXFLAGS=-std=c++17 -fno-operator-names -ggdb
LINK.o=$(LINK.cc)

main: main.o concept.o helpers.o memorystore.o
*.o: *.hpp
clean:
	-rm *.o main
