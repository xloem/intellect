#include "sugar.hpp"

#include "concepts.hpp"

#include <stdexcept>

using namespace intellect::level1;
using namespace concepts;

namespace intellect {
namespace level1 {

ref a(ref group)
{
	static unsigned long long gid = 0;
	ref ret(group.name() + "-" + std::to_string(gid++));
	ret.link(is, group);
	ret.link(is, anonymous);
	return ret;
}
ref a(ref group, ref name)
{
	if (!name.isa(group)) {
		name.link(is, group);
	}
	return name;
}
ref an(ref group)
{
	return a(group);
}
ref an(ref group, ref name)
{
	return a(group, name);
}

bool isanonymous(ref topic)
{
	return topic.isa(concepts::anonymous);
}

ref movetoname(ref anonymous, ref name)
{
	if (!isanonymous(anonymous)) { throw std::invalid_argument("not anonymous"); }
	if (isanonymous(name)) { throw std::invalid_argument("not named"); }
	
	// this only provides for writing to empty concepts, because merging concepts is
	// best done with a knowledge of which links can be multiply attached, and that
	// information is not available at this level.
	bool nonempty = false;
	for (auto & l : name.links()) {
		if (l.first == concepts::name) { continue; }
		nonempty = true;
	}
	if (nonempty) {
		for (auto & link : anonymous.links()) {
			if (link.first == concepts::is && link.second == concepts::anonymous) { continue; }
			if (link.first == concepts::name) { continue; }
			if (!name.linked(link.first, link.second)) {
				throw std::logic_error(name.name() + " already defined otherwise from " + anonymous.get(concepts::is).name());
			}
		}
	}
	anonymous.unlink(concepts::is, concepts::anonymous);
	auto nam = anonymous.get(concepts::name);
	anonymous.unlink(concepts::name, nam);
	if (!nonempty) {
		for (auto & l : anonymous.links()) {
			name.link(l.first, l.second);
		}
	}
	anonymous.link(concepts::name, nam);
	dealloc(anonymous);
	dealloc(nam);
	return name;
}

}
}
