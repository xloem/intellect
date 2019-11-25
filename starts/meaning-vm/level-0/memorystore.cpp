#include "memorystore.hpp"
#include "concept.hpp"
#include "errors.hpp"

#include <unordered_set>

namespace intellect {
namespace level0 {

static auto & concepts()
{
	static std::unordered_set<concept*, std::hash<concept*>> concepts;
	return concepts;
}

concept* alloc(concept * moved) {
	ref r = moved ? moved : new concept();
	concepts().insert(r);
	return r;
}

static concept* referenced(ref r) {
	for (ref r2 : concepts()) {
		if (r2 == r) {
			continue;
		}
		for (auto & l : r2.links()) {
			if (ref(l.first) == r) {
				return r2;
			}
			if (ref(l.second) == r) {
				return r2;
			}
		}
	}
	return 0;
}

void dealloc(concept * r) {
	concept * referenced = intellect::level0::referenced(r);
	if (referenced) {
		throw still_referenced_by(r, referenced);
	}
	for (
		auto it = concepts().begin();
		it != concepts().end();
		++ it)
	{
		if (*it == r) {
			concepts().erase(it);
			delete (concept*)r;
			return;
		}
	}
	throw no_such_concept(r);
}

std::size_t allocated()
{
	return concepts().size();
}

}
}
