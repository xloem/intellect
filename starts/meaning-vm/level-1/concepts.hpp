#pragma once

#include "ref.hpp"
#include "sugar.hpp"

namespace intellect {
namespace level1 {

namespace concepts {

extern ref name; // used as the link to value<std::string> naming each concept
extern ref is; // a link to define group relationships, links to the more general class
extern ref anonymous; // a group given concepts with generated names
extern ref link; // TODO: for concepts that are links, link them with is=link
//extern ref true, false; <-- casting provides as if these were declared

}

}
}
