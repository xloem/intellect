#include "concepts.hpp"

#include "funcs.hpp"
#include "../level-0/memorystore.hpp"

namespace intellect {
namespace level1 {
namespace concepts {

static struct init { init() {
	givename(intellect::level0::concepts::allocator(), "allocator");
	givename(intellect::level0::concepts::allocates(), "allocates");
	givename(intellect::level0::concepts::allocations(), "allocations");
	givename(intellect::level0::concepts::level0allocations(), "level0-allocations");
} } __init;

ref level1("level1"), allocations("allocations");
ref is("is", level1-allocations);
ref name("name", level1-allocations);
ref nothing("nothing", level1-allocations);
ref anonymous("anonymous", level1-allocations);
ref link("link", level1-allocations);
ref level0("level0", level1-allocations) , level2("level2", level1-allocations) , level3("level3", level1-allocations) , level4("level4", level1-allocations) , level5("level5", level1-allocations) , level6("level6", level1-allocations) , level7("level7", level1-allocations) , level8("level8", level1-allocations) , level9("level9", level1-allocations);
ref allocator("allocator"), allocates("allocates");

ref _false("false", level1-allocations), _true("true", level1-allocations);

}
}
}
