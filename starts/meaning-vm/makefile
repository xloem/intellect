CXXFLAGS=-Wall -Werror -std=c++17 -fno-operator-names -ggdb -O0
LINK.o=$(LINK.cc)

all: level0 level1 habit-starts/rhythm
level0: level0.o liblevel0.a
liblevel0.a: level-0/memorystore.o level-0/concept.o level-0/ref.o
level1: level1.o liblevel1.a liblevel0.a
liblevel1.a: level-1/ref.o level-1/sugar.o level-1/concepts.o level-1/funcs.o

habit-starts/rhythm: habit-starts/rhythm.o liblevel1.a liblevel0.a

liblevel%.a: level-%/*.hpp
%.a:
	ar ru $@ $^
	ranlib $@

clean:
	-rm *.o level? *.a */*.o
