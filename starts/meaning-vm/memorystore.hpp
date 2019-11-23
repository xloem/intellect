#pragma once

#include "concept.hpp"

ref alloc(concept * moved = 0);
template <typename T>
vref<T> valloc(T const & val) { return alloc(new value<T>(val)); }
void dealloc(ref);
void dealloc_all(ref); // todo?
