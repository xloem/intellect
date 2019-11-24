#include "memorystore.hpp"

#include <unordered_set>

using namespace std;

auto & concepts()
{
	static std::unordered_set<concept*> concepts;
	return concepts;
}

ref alloc(concept * moved) {
	ref r = moved ? moved : new concept();
	concepts().insert(r.ptr);
	return r;
}

concept* referenced(ref r) {
	for (ref r2 : concepts()) {
		if (r2 == r) {
			continue;
		}
		for (auto & l : r2->links) {
			if (ref(l.first) == r) {
				return r2.ptr;
			}
			if (ref(l.second) == r) {
				return r2.ptr;
			}
		}
	}
	return 0;
}

void dealloc(ref r) {
	concept * referenced = ::referenced(r);
	if (referenced) {
		throw std::logic_error("concept '" + r.name()->data + "' is referenced by '" + ref(referenced).name()->data + '"');
	}
	for (
		auto it = concepts().begin();
		it != concepts().end();
		++ it)
	{
		if (ref(*it) == r) {
			concepts().erase(it);
			delete r.ptr;
			return;
		}
	}
	throw std::logic_error("concept not held");
}
