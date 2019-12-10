#include "memorystore.hpp"
#include "concept.hpp"
#include "errors.hpp"
#include "ref.hpp"

#include <memory>
#include <unordered_map>

namespace intellect {
namespace level0 {

static auto & index()
{
	static std::unordered_map<ref, std::unique_ptr<concept>, std::hash<concept*>> index;
	return index;
}


namespace concepts {
	ref allocator() { static ref ret = basic_alloc(); return ret; };
	ref allocates() { static ref ret = basic_alloc(); return ret; };
	ref allocations() { static ref ret = basic_alloc(); return ret; };
	ref level0allocations() { static ref ret = basic_alloc(); return ret; };
}

struct init { init()
{
	concepts::allocator().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::allocator());

	concepts::allocates().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::allocates());

	concepts::allocations().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::allocations());

	concepts::level0allocations().link(concepts::allocator(), concepts::level0allocations());
	concepts::level0allocations().link(concepts::allocates(), concepts::level0allocations());
} } _init;

ref basic_alloc(std::any data)
{
	ref r = new concept();
	r.ptr()->data = data;
	index().emplace(r, r.ptr());
	return r;
}

ref alloc(ref source, std::any data)
{
	ref r = basic_alloc(data);
	alloc(r, source);
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

static concept* referenced(ref r, concept* source = 0) {
	for (auto & r2pair : index()) {
		ref r2 = r2pair.first;
		if (r2.ptr() == source) {
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

void basic_dealloc(ref r)
{
	auto it = index().find(r);
	if (it == index().end()) { throw no_such_concept(r); }

	concept * referenced = intellect::level0::referenced(r);
	if (referenced) {
		throw still_referenced_by(r, referenced);
	}

	index().erase(it);
}

void dealloc_from(ref source)
{
	std::remove_reference<decltype(index())>::type forgotten;

	auto ours = source.getAll(concepts::allocates());
	for (auto allocation : ours) {
		source.unlink(concepts::allocates(), allocation);
		allocation.unlink(concepts::allocator(), source);
		if (allocation.linked(concepts::allocator())) { continue; }

		auto it = index().find(allocation);
		if (it != index().end()) {
			forgotten.insert(index().extract(it));
		}
	}
	try {
		for (auto allocation : ours ) {
			if (allocation.linked(concepts::allocates())) {
				dealloc_from(allocation);
			}
		}
		for (auto ghost : ours) {
			concept * referenced = intellect::level0::referenced(ghost, source);
			if (referenced) {
				throw still_referenced_by(ghost, referenced);
			}
		}
	} catch(...) {
		// NOTE: this doesn't rebuild deallocated subgroups, but that could be done
		// by returning them.
		index().merge(forgotten);
		for (auto allocation : ours) {
			source.link(concepts::allocates(), allocation);
			allocation.link(concepts::allocator(), source);
		}
		throw;
	}

	// concepts in forgotten will be deallocated when they leave scope
	// note: scoped allocation is just a plan to forget (at the end of a { } block)
}

void dealloc(ref r, ref source)
{
	auto it = index().find(r);
	if (it == index().end()) { throw no_such_concept(r); }

	source.unlink(concepts::allocates(), r);
	r.unlink(concepts::allocator(), source);
	if (r.linked(concepts::allocator())) { return; }

	try {
		dealloc_from(r);
		concept * referenced = intellect::level0::referenced(r, source);
		if (referenced) {
			throw still_referenced_by(r, referenced);
		}

		index().erase(it);
	} catch(...) {
		source.link(concepts::allocates(), r);
		r.link(concepts::allocator(), source);
	}
}

std::size_t allocated()
{
	return index().size();
}

}
}
