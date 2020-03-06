project(RunningRelevence)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# -mcmodel=large forces far calls on 64-bit.  otherwise they must be in the same segment.
# some other platforms have -mlong-calls
add_compile_options(-fno-plt -mcmodel=large)

## on 64-bit, near calls are a 32-bit offset from the instruction pointer.
## on 32-bit, near calls are a 16-bit offset from the 16-bit segment register.
## so copying functions should copy all the nearby libraries too.
#### additionally, this means functions cannot be moved.
#### if making a function inspires making a nearby similar function, all the addresses will be different.
#### they should be normalized either via disassembly are adjustment.
#### alternatively we could only alter things in-place, or we could alter .so files that are loaded from the disk or ram.

add_executable(running_relevence running_relevence.cpp)
