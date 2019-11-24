#pragma once

#include "common.hpp"
#include "ref.hpp"

namespace intellect {
namespace level0 {

ref alloc(concept * moved = 0);
void dealloc(ref);
std::size_t allocated();

}
}
