#include "memorystore.hpp"

#include <list>
#include <vector>

using namespace std;

std::vector<ref> concepts;

ref alloc(concept * moved) {
	ref r = moved ? moved : new concept();
	concepts.push_back(r);
	return r;
}

bool referenced(ref r) {
	for (auto & r2 : concepts) {
		if (r2 == r) {
			continue;
		}
		for (auto & l : r2->links) {
			if (l.first == r) {
				return true;
			}
			if (l.second == r) {
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
		auto it = concepts.begin();
		it != concepts.end();
		++ it)
	{
		if (*it == r) {
			concepts.erase(it);
			delete r;
			return;
		}
	}
	throw std::logic_error("concept not held");
}
