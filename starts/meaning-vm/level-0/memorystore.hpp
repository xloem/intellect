#pragma once

#include "common.hpp"
#include "value.hpp"

namespace intellect {
namespace level0 {

concept * alloc(concept * moved = 0);
template <typename T>
value<T> * valloc(T const & v) { return static_cast<value<T>*>(alloc(new value<T>(v))); }
void dealloc(concept*);
std::size_t allocated();

}
}
