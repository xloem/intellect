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

%.o: %.ii %.cpp
	$(CXX) $(CXXFLAGS) -c $*.ii -o $*.o
%.d: %.cpp
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $*.d -MT "$*.ii $*.o $*.d" -E $*.cpp
%.ii: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $*.cpp -o $*.ii
.INTERMEDIATE: $(patsubst %.cpp,%.ii,$(wildcard */*.cpp) $(wildcard *.cpp))

-include $(patsubst %.cpp,%.d,$(wildcard level-0/*.cpp) $(wildcard level-1/*.cpp) $(wildcard level-2/*.cpp) $(wildcard level*.cpp))

%.a:
	ar cr $@ $^
	ranlib $@

clean:
	-rm *.ii *.o level? *.a */*.o */*.ii *.d */*.d
