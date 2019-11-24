CXXFLAGS=-Wall -Werror -std=c++17 -fno-operator-names -ggdb -O0
LINK.o=$(LINK.cc)

level0: level0.o liblevel0.a
liblevel0.a: level-0/memorystore.o level-0/concept.o level-0/ref.o
%.a:
	ar ru $@ $^
	ranlib $@

main: main.o concept.o helpers.o memorystore.o meaning.o
*.o: *.hpp  */*.hpp
clean:
	-rm *.o main level0 *.a */*.o
