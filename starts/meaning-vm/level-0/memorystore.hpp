#pragma once

#include "common.hpp"

#include <any>

namespace intellect {
namespace level0 {

// self-reference loops are real.
//
// one person can feel urgent about issue A, and act on this urgency to engage
// another person around B, who acts in a different way to someone else, eventually
// cycling back to stress that stimulates the original person to feel more urgent
// about issue A.
// 	human behavior can make arbitrary positive or negative feedback loops.
//
// here in memory allocation, i've designed a system intended to reduce such loops
// by encouraging my usage to be a certain way, but it still readily provides for
// them.
//
// in process expansion / simple thought, we also have the issue of recursion.
// if we trust a task to complete, and it ends up triggering itself in a subcontext,
// we could wait forever.
//
// the solution to many of these things is to recognize repetition in systems.
// we also become skeptical as things continue constantly.  we expect to develop
// some level of understanding that they will shrink, or we stop them and try
// something else.

namespace concepts {

extern ref allocator(); // link shows what is holding something alive
extern ref allocates(); // link shows what is being held alive

extern ref allocations(); // to use as a basic allocator for simple things
extern ref level0allocations(); // allocator for concepts internal to level0

}

ref alloc(ref allocator, std::any data = {}); // new concept
void alloc(ref allocated, ref allocator); // extra ownership for concept
void realloc(ref allocated, ref allocator); // move ownership for concept to allocator
void dealloc(ref allocated, ref allocator); // remove ownership for concept
std::size_t allocated();

}
}
