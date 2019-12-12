#pragma once

#include "common.hpp"

#include "../level-0/memorystore.hpp"

#include <string>
#include <sstream>
#include <typeinfo>

namespace intellect {
namespace level1 {

concept* getnamed(std::string const & name, concept* allocator = nullptr);
std::string getname(concept* r);
void givename(concept* con, std::string const & name);

bool isa(concept* member, concept* group);

concept* hyphenate(concept* a, concept* b);

concept* alloc(concept* allocator, std::any val);

std::string dump(concept * what, concept * skipmarkertype, concept * skipmarkertarget);

}
}
