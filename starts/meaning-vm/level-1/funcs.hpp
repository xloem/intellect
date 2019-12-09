#pragma once

#include "common.hpp"

#include "../level-0/memorystore.hpp"

#include <string>
#include <sstream>
#include <typeinfo>

namespace intellect {
namespace level1 {

concept* getnamed(std::string const & name);
std::string& getname(concept* r);

bool isa(concept* member, concept* group);

concept* hyphenate(concept* a, concept* b);

concept* alloc(std::any val);

std::string dump(concept * what, concept * skipmarkertype, concept * skipmarkertarget);

}
}
