CXXFLAGS=-Wall -Werror -std=c++17 -fno-operator-names -ggdb -O0
LINK.o=$(LINK.cc)

all: level0 level1 habit-starts/rhythm
level0: level0.o liblevel0.a
liblevel0.a: $(patsubst %.cpp,%.o,$(wildcard level-0/*.cpp))
level1: level1.o liblevel1.a
liblevel1.a: $(patsubst %.cpp,%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard level-1/*.cpp))
level2: liblevel2.a
liblevel2.a: $(patsubst %.cpp,%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard level-1/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard level-2/*.cpp))

habit-starts/rhythm: habit-starts/rhythm.o liblevel2.a

liblevel%.a: level-%/*.hpp
%.a:
	ar ru $@ $^
	ranlib $@

clean:
	-rm *.o level? *.a */*.o
