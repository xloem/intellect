#include "ref.hpp"
#include "concept.hpp"
#include "errors.hpp"
#include "memorystore.hpp"

#include <iomanip>
#include <ostream>
#include <sstream>

using namespace intellect::level0;
using namespace concepts;

std::string ref::dump(ref skipmarkertype, ref skipmarkertarget)
{
	if (self.linked(skipmarkertype, skipmarkertarget)) {
		return {};
	}
	std::stringstream ss;
	ss << std::hex << (size_t)ptr() << ":" << std::endl;
	for (auto & link : self.links()) {
		if (link.first.linked(allocator(), level0allocations())) { continue; }
		ss << "  " << (size_t)link.first.ptr() << ": " << (size_t)link.second.ptr() << std::endl;
	}
	self.link(skipmarkertype, skipmarkertarget);
	for (auto & link : self.links()) {
		if (link.first.linked(allocator(), level0allocations())) { continue; }
		if (link.first == skipmarkertype && link.second == skipmarkertarget) {
			continue;
		}
		ss << link.second.dump(skipmarkertype, skipmarkertarget);
	}
	return ss.str();
}
