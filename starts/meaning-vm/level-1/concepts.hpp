#pragma once

#include "ref.hpp"

namespace intellect {
namespace level1 {

namespace concepts {

extern ref nothing; // default value of a ref
extern ref name; // used as the link to value<std::string> naming each concept
extern ref is; // a link to define group relationships, links to the more general class
extern ref anonymous; // a group given concepts with generated names
extern ref link; // TODO: for concepts that are links, link them with is=link
extern ref level0, level1, level2, level3, level4, level5, level6, level7, level8, level9, level10;
extern ref allocator, allocates, allocations;

//extern ref true, false; <-- casting provides as if these were declared

}

}
}
