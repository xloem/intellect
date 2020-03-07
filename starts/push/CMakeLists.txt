project(RunningRelevance)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# it looks like when -fno-plt is used, calls are made into a table in the same segment as the code,
# relative address for each call.  When it is not used, the function address is loaded in at the start
# of each function, and values are added to it to find the spot in the table.
# this table is filled with the correct call locations on loading.

# -mcmodel=large forces far calls on 64-bit.  otherwise they must be in the same segment.
# some other platforms have -mlong-calls

#add_compile_options(-fno-plt -mcmodel=large)
#add_compile_options(-mcmodel=large)

## on 64-bit, near calls are a 32-bit offset from the instruction pointer.
## on 32-bit, near calls are a 16-bit offset from the 16-bit segment register.
## so copying functions should copy all the nearby libraries too.
#### additionally, this means functions cannot be moved.
#### if making a function inspires making a nearby similar function, all the addresses will be different.
#### they should be normalized either via disassembly are adjustment.
#### alternatively we could only alter things in-place, or we could alter .so files that are loaded from the disk or ram.
####### what is wrong with far calls?

add_library(loaded_relevance SHARED loaded_relevance.cpp)
add_library(find_relevant_string SHARED find_relevant_string.cpp)
add_library(copy_data SHARED copy_data.cpp)
add_library(core SHARED core.cpp)
add_library(example_behavior SHARED example_behavior.cpp)
add_library(run SHARED run.cpp)
add_library(running_relevence SHARED running_relevance.cpp)
target_link_libraries(core find_relevant_string)

add_executable(running_relevance running_relevance.cpp)
target_link_libraries(running_relevance dl loaded_relevance find_relevant_string copy_data core example_behavior run running_relevence)

