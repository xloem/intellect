#pragma once

#include "ref.hpp"

namespace intellect {
namespace level1 {

namespace concepts {

static ref level1("level1"), allocations("allocations");
static ref is("is", level1-allocations); // a link to define group relationships, links to the more general class
static ref name("name", level1-allocations); // used as the link to value<std::string> naming each concept
static ref nothing("nothing", level1-allocations); // default value of a ref
static ref anonymous("anonymous", level1-allocations); // a group given concepts with generated names
static ref link("link", level1-allocations); // TODO: for concepts that are links, link them with is=link
static ref level0("level0", level1-allocations) , level2("level2", level1-allocations) , level3("level3", level1-allocations) , level4("level4", level1-allocations) , level5("level5", level1-allocations) , level6("level6", level1-allocations) , level7("level7", level1-allocations) , level8("level8", level1-allocations) , level9("level9", level1-allocations);
static ref allocator("allocator"), allocates("allocates");

static ref _false("false", level1-allocations), _true("true", level1-allocations);

//extern ref true, false; <-- casting provides as if these were declared

}

}
}
