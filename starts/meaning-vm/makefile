# Please support Karl working on this intelligence framework.  The framework needs to be quality, so that the rest may function.

CXXFLAGS=-pedantic -Wall -Werror -Wno-error=deprecated-declarations -Wno-gnu-zero-variadic-macro-arguments -std=gnu++17 -fno-operator-names -ggdb -fno-omit-frame-pointer -O0
#-Wno-error=unused-command-line-argument 
#-fsanitize-memory-track-origins -fsanitize=memory -fPIE -pie -v
LINK.o=$(LINK.cc) -v
OBJDIR=$(TMPDIR)/intellect-vm-objs/

all: level0 level1 habit-starts/rhythm level2 level3
level0: level0.o liblevel0.a
liblevel0.a: $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-0/*.cpp))
level1: level1.o liblevel1.a
liblevel1.a: $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-1/*.cpp))
level2: level2.o liblevel2.a
liblevel2.a: $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-1/*.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-2/*.cpp))
level3: level3.o liblevel3.a
liblevel3.a: $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-0/*.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-1/*.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-2/*.cpp)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(wildcard level-3/*.cpp))

habit-starts/rhythm: habit-starts/rhythm.o liblevel2.a

%.o: $(OBJDIR)/%.o
	ln -sf $(OBJDIR)/$*.o
$(OBJDIR)/%.o: $(OBJDIR)/%.ii %.cpp
	$(CXX) $(CXXFLAGS) -c $(OBJDIR)/$*.ii -o $(OBJDIR)/$*.o
$(OBJDIR)/%.d: %.cpp
	@if ! [ -d $(dir $@) ]; then mkdir $(dir $@); fi
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM -MF $(OBJDIR)/$*.d -MT "$(OBJDIR)/$*.ii $(OBJDIR)/$*.o $(OBJDIR)/$*.d" -E $*.cpp
$(OBJDIR)/%.ii: %.cpp $(OBJDIR)/%.d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -E $*.cpp -o $(OBJDIR)/$*.ii
.INTERMEDIATE: $(patsubst %.cpp,$(OBJDIR)/%.ii,$(wildcard */*.cpp) $(wildcard *.cpp))

-include $(patsubst %.cpp,$(OBJDIR)/%.d,$(wildcard level-0/*.cpp) $(wildcard level-1/*.cpp) $(wildcard level-2/*.cpp) $(wildcard level*.cpp))

%.a:
	ar cr $@ $^
	ranlib $@

clean:
	-rm $(OBJDIR)/*.ii $(OBJDIR)/*.o level? *.a $(OBJDIR)/*/*.o $(OBJDIR)/*/*.ii $(OBJDIR)/*.d $(OBJDIR)/*/*.d
