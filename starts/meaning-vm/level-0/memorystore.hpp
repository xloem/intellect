#pragma once

#include "common.hpp"
#include "value.hpp"

namespace intellect {
namespace level0 {

ref alloc(concept * moved = 0);
template <typename T>
value<T> * valloc(T const & v) { return static_cast<value<T>*>(alloc(new value<T>(v)).ptr()); }
void dealloc(ref);
std::size_t allocated();

}
}
