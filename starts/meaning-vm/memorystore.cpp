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

bool referenced(ref r) {
	for (ref r2 : concepts()) {
		if (r2 == r) {
			continue;
		}
		for (auto & l : r2->links) {
			if (ref(l.first) == r) {
				return true;
			}
			if (ref(l.second) == r) {
				return true;
			}
		}
	}
	return false;
}

void dealloc(ref r) {
	if (referenced(r)) {
		throw std::logic_error("concept is referenced");
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
