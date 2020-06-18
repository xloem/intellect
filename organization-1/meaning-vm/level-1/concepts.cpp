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


}
}
}
