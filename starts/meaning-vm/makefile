CXXFLAGS=-std=c++17 -fno-operator-names -ggdb -O0
LINK.o=$(LINK.cc)

main: main.o concept.o helpers.o memorystore.o meaning.o
*.o: *.hpp
clean:
	-rm *.o main
