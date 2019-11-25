#include "ref.hpp"

#include "concepts.hpp"


using namespace intellect;
using namespace level1;
using namespace concepts;

std::string ref::dump(ref skipmarkertype, ref skipmarkertarget)
{
	if (linked(skipmarkertype, skipmarkertarget)) {
		return {};
	}
	std::string ret;
	for (auto & link : links()) {
		if (link.first == concepts::name) { continue; }
		if (ret.size() == 0) {
			ret = name() + ":\n";
		}
		ret += "  " + link.first.name() + ": " + link.second.name() + "\n";
	}
	link(skipmarkertype, skipmarkertarget);
	for (auto & link : links()) {
		if (link.first == skipmarkertype && link.second == skipmarkertarget) {
			continue;
		}
		ret += link.second.dump(skipmarkertype, skipmarkertarget);
	}
	return ret;
}
