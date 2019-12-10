#include "ref.hpp"
#include "concept.hpp"
#include "errors.hpp"
#include "memorystore.hpp"

#include <ostream>

using namespace intellect::level0;
using namespace concepts;

std::string ref::dump(ref skipmarkertype, ref skipmarkertarget)
{
	if (self.linked(skipmarkertype, skipmarkertarget)) {
		return {};
	}
	std::string ret = std::to_string((unsigned long)ptr()) + ":\n";
	for (auto & link : self.links()) {
		if (link.first.linked(allocator(), level0allocations())) { continue; }
		ret += "  " + std::to_string((unsigned long)link.first.ptr()) + ": " + std::to_string((unsigned long)link.second.ptr()) + "\n"; 
	}
	self.link(skipmarkertype, skipmarkertarget);
	for (auto & link : self.links()) {
		if (link.first.linked(allocator(), level0allocations())) { continue; }
		if (link.first == skipmarkertype && link.second == skipmarkertarget) {
			continue;
		}
		ret += link.second.dump(skipmarkertype, skipmarkertarget);
	}
	return ret;
}
