#pragma once

#include "common.hpp"

#include "../level-0/memorystore.hpp"

#include <string>
#include <sstream>
#include <typeinfo>

namespace intellect {
namespace level1 {

concept* getnamed(std::string const & name);
value<std::string>* getname(concept* r);

bool isa(concept* member, concept* group);

template <typename T>
value<T>* valloc(T const & val);

}
}
