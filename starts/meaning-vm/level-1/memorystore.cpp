#include "memorystore.hpp"

#include "concepts.hpp"

#include "../level-0/memorystore.hpp"

namespace intellect {
namespace level1 {

ref allocation_group("allocation-group");
ref allocation_part("allocation-part");

ref alloc(ref grp, std::any data)
{
	ref res = level0::alloc(data);
	keep(res, grp);
}

void keep(ref allocated, ref grp)
{
	allocated.link(allocation_group, grp);
	grp.link(allocation_part, allocated);
}

void dealloc(ref grp, ref metagroup)
{

//}

std::size_t allocatedgroups();

}
}
