#pragma once

#include "common.hpp"

#include <any>

namespace intellect {
namespace level0 {

ref alloc(std::any data = {});
void dealloc(ref);
std::size_t allocated();

}
}
