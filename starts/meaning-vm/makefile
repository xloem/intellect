# Please support Karl working on this intelligence framework.  The framework needs to be quality, so that the rest may function.

CXXFLAGS=-pedantic -Wall -Werror -Wno-error=deprecated-declarations -Wno-gnu-zero-variadic-macro-arguments -std=gnu++17 -fno-operator-names -ggdb -O0
LINK.o=$(LINK.cc)

all: level0 level1 habit-starts/rhythm level2
level0: level0.o liblevel0.a
liblevel0.a: $(patsubst %.cpp,%.o,$(wildcard level-0/*.cpp))
level1: level1.o liblevel1.a
liblevel1.a: $(patsubst %.cpp,%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard level-1/*.cpp))
level2: level2.o liblevel2.a
liblevel2.a: $(patsubst %.cpp,%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard level-1/*.cpp)) $(patsubst %.cpp,%.o,$(wildcard level-2/*.cpp))

habit-starts/rhythm: habit-starts/rhythm.o liblevel2.a

%.o: %.ii
	$(CXX) $(CXXFLAGS) -c $^ -o $@
%.ii: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $< -o $@
.INTERMEDIATE: $(patsubst %.cpp,%.ii,$(wildcard */*.cpp) $(wildcard *.cpp))

level-0/%.ii: $(wildcard level-0/*.hpp)
level-1/%.ii: $(wildcard level-0/*.hpp) $(wildcard level-1/*.hpp)
level-2/%.ii: $(wildcard level-0/*.hpp) $(wildcard level-1/*.hpp) $(wildcard level-2/*.hpp)

%.a:
	ar cr $@ $^
	ranlib $@

clean:
	-rm *.ii *.o level? *.a */*.o */*.ii
