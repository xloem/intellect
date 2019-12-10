#include "memorystore.hpp"
#include "concept.hpp"
#include "errors.hpp"
#include "ref.hpp"

#include <unordered_set>

namespace intellect {
namespace level0 {

static auto & index()
{
	static std::unordered_set<ref, std::hash<concept*>> index;
	return index;
}


namespace concepts {
	ref allocator() { static ref ret = new concept(); return ret; };
	ref allocates() { static ref ret = new concept(); return ret; };
	ref allocations() { static ref ret = new concept(); return ret; };
	ref level0allocations() { static ref ret = new concept(); return ret; };
}

struct init { init()
{
	concepts::allocator().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::allocator());
	index().insert(concepts::allocator());

	concepts::allocates().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::allocates());
	index().insert(concepts::allocates());

	concepts::allocations().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::allocations());
	index().insert(concepts::allocations());

	concepts::level0allocations().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::level0allocations());
	index().insert(concepts::level0allocations());
} } _init;

ref alloc(ref source, std::any data)
{
	concept * r = new concept();
	r->data = data;
	alloc((ref)r, source);
	index().insert(r);
	return r;
}

void alloc(ref r, ref source)
{
	r.link(concepts::allocator(), source);
	source.link(concepts::allocates(), r);
}

void realloc(ref r, ref newsource)
{
	ref oldsource = r.get(concepts::allocator());
	alloc(r, newsource);
	dealloc(r, oldsource);
}

static concept* referenced(ref r, ref source) {
	for (ref r2 : index()) {
		if (r2 == source) {
			continue;
		}
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

void dealloc(ref r, ref source) {
	auto it = index().find(r);
	if (it == index().end()) { throw no_such_concept(r); }

	source.unlink(concepts::allocates(), r);
	r.unlink(concepts::allocator(), source);
	if (r.linked(concepts::allocator())) { return; }

	index().erase(it);

	auto ours = r.getAll(concepts::allocates());
	for (auto allocation : ours) {
		dealloc(allocation, r);
	}

	for (auto ghost : ours) {
		concept * referenced = intellect::level0::referenced(ghost, source);
		if (referenced) {
			throw still_referenced_by(ghost, referenced);
		}
	}
	delete (concept*)r;
}

std::size_t allocated()
{
	return index().size();
}

}
}
