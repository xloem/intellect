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

// A solution to recursion appears to involve emotional expression.
// Too much recursion maps acceptably to frustration of the process doing the repetitive task.
// The building unmet need for effectiveness should influence other decision-making processes
// if nothing else is larger.  Notably if the caller needs timeliness, they won't get this
// if the callee(s) do not have effectiveness.
// 	propose: raise frustration[effectiveness] when calling self or repeating same behavior
// 	propose: raise frustration[timeliness] if subprocess takes long (say every 400ms)
// 	ideally raising an emotional expression should be associated with what caused it
// 	and how the the universe might change to fix it.
// decision-making processes need to judge what is relevent to them: a product of how well
// they can help something and how strongly it is needed.

namespace concepts {

extern ref allocator(); // link shows what is holding something alive
extern ref allocates(); // link shows what is being held alive

extern ref allocations(); // to use as a basic allocator for simple things
extern ref level0allocations(); // allocator for concepts internal to level0

}

ref basic_alloc(std::any data = {});
void basic_dealloc(ref allocated);

// Below functions manage allocation assuming ownership.
// When all ownership is removed from a concept,
// it is deallocated, and relinquishes everything it owns.
// The last owner of an in-use concept is prevented from
// deallocating it until it is no longer in use.
ref alloc(ref owner, std::any data = {}); // new concept
bool alloced(ref concept, ref owner); // discern whether concept is owned by owner
void alloc(ref concept, ref owner); // extra ownership for concept
void realloc(ref concept, ref owner); // move ownership for concept to new owner
void dealloc(ref concept, ref owner); // remove ownership for concept

// To track leaks, total number of concepts allocated
std::size_t allocated();

}
}
