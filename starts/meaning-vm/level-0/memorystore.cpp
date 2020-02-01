#include "memorystore.hpp"
#include "concept.hpp"
#include "errors.hpp"
#include "ref.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>

namespace intellect {
namespace level0 {

static auto & guid()
{
	static std::unordered_map<ref, uint64_t, std::hash<concept*>> guid;
	return guid;
}

static auto & index()
{
	static struct ist {
		std::unordered_map<ref, std::unique_ptr<concept>, std::hash<concept*>> index;
		~ist() {
			for (auto & item : index) {
				auto & links = item.second->links;
				for (auto it = links.begin(); it != links.end();) {
					item.second->unlink(it++);
				}
			}
		}
	} index_struct;
	return index_struct.index;
}


namespace concepts {
	ref allocator() { static ref ret = basic_alloc(); return ret; }
	ref allocates() { static ref ret = basic_alloc(); return ret; }
	ref allocations() { static ref ret = basic_alloc(); return ret; }
	ref level0allocations() { static ref ret = basic_alloc(); return ret; }
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

/*
 * this is a nice pool idea i guess.  can use sysconf(_SC_PAGE_SIZE) to get pagesize
 * possibly.  but did not implement deallocation using pointer chain, so not usable
 * yet.  could just use concept links to do chain.
template <typename T>
class pool {
public:
	static constexpr size_t pagesize() { return 4096*1024; }
	static constexpr size_t rowlength() { return pagesize() / sizeof(T); }
	static constexpr size_t alignment() { return pagesize(); }
	pool()
	{
		lastct = 0;
		newrow();
	}
	T* alloc()
	{
		if (idx >= rowlength()) {
			newrow();
		}
		return &(*last)[idx ++];
	}
	uint64_t id(T* v)
	{
		uintptr_t addr = v;
		uintptr_t offset = v % alignment();
		uintptr_t base = addr - offset;
		return lookup[base] + (offset / sizeof(T));
	}
private:
	void newrow()
	{
		last = new(std::aligned_alloc(alignment(), alignment())) std::array<T,rowlength()>();
		//storage.emplace_back();
		//last = &storage.back();
		idx = 0;
		assert(uintptr_t(last) % alignment() == 0);
		lookup[uintptr_t(last)] = lastct;
		lastct += rowlength();
	}
	size_t idx;
	uint64_t lastct;
	std::array<T,rowlength()> * last;
	std::unordered_map<uintptr_t,uint64_t> lookup;
};
*/

std::mutex indices;

ref basic_alloc(std::any data)
{
	//static pool<concept> concept_pool;
	ref r = new concept();//concept_pool.alloc();//new concept();
	// i think this segfaults because it can get called before cerr is init'd
	//std::cerr << "alloc: " << std::hex << (uint64_t)r.ptr() << std::endl;
	r.ptr()->data = data;
	std::lock_guard lk(indices);
	index().emplace(r, r.ptr());
	static uint64_t lastguid = 0;
	guid().emplace(r, lastguid++);
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
	if (r.ptr()->refcount() == 0) { return 0; }
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
	if (r.crucial()) { throw crucial_concept(r); }
	auto it = index().find(r);
	if (it == index().end()) { throw no_such_concept(r); }

	concept * referenced = intellect::level0::referenced(r);
	if (referenced) {
		throw still_referenced_by(r, referenced);
	}

	//if (r.ptr()->refcount() != 0) {
	//	throw still_referenced(r);
	//}

	std::lock_guard lk(indices);
	guid().erase(it->first);
	index().erase(it);
}

void dealloc_from(ref source)
{
	std::lock_guard lk(indices);
	std::remove_reference<decltype(index())>::type forgotten;
	std::vector<std::tuple<ref,ref,ref>> forgotten_links;

	auto ours = source.getAll(concepts::allocates());
	for (auto oursit = ours.begin(); oursit != ours.end();) {
		auto allocation = *(oursit++);
		if (allocation.crucial()) { throw crucial_concept(allocation); }
		source.unlink(concepts::allocates(), allocation);
		allocation.unlink(concepts::allocator(), source);
		if (allocation.linked(concepts::allocator())) { continue; }

		auto it = index().find(allocation);
		//if (it != index().end()) {
			forgotten.insert(index().extract(it));
		//}

		auto links = allocation.links();
		for (auto it = links.begin(); it != links.end();) {
			forgotten_links.emplace_back(allocation, it->first, it->second);
			allocation.unlink(it++);
		}
	}
	try {
		for (auto & allocations : forgotten ) {
			auto & allocation = allocations.first;
			for (auto suballocation : allocation.getAll(concepts::allocates())) {
				// check for this link to find subgroups
				throw still_referenced_by(allocation, suballocation);
			}
		}
		for (auto & ghostpair : forgotten) {
			auto & ghost = ghostpair.first;
			
			concept * referenced = intellect::level0::referenced(ghost, source);
			if (referenced) {
				throw still_referenced_by(ghost, referenced);
			}
			
			//if (ghost.ptr()->refcount() != 0) {
			//	throw still_referenced(ghost);
			//}
		}
	} catch(...) {
		// NOTE: this doesn't rebuild deallocated subgroups, but that could be done
		// by returning them.
		for (auto link : forgotten_links) {
			std::get<0>(link).link(std::get<1>(link), std::get<2>(link));
		}
		for (auto & allocations : forgotten) {
			auto & allocation = allocations.first;
			source.link(concepts::allocates(), allocation);
			allocation.link(concepts::allocator(), source);
		}
		index().merge(forgotten);
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

	std::vector<std::pair<ref,ref>> removed_links;

	try {
		if (r.crucial()) { throw crucial_concept(r); }
		auto links = r.links();
		for (auto it = links.begin(); it != links.end();) {
			if (it->first == concepts::allocates()) { ++ it; continue; }
			removed_links.push_back(*it);
			r.unlink(it++);
		}
		dealloc_from(r);
		concept * referenced = intellect::level0::referenced(r, source);
		if (referenced) {
			throw still_referenced_by(r, referenced);
		}

		std::lock_guard lk(indices);
		guid().erase(it->first);
		index().erase(it);
	} catch(...) {
		source.link(concepts::allocates(), r);
		r.link(concepts::allocator(), source);
		for (auto link : removed_links) {
			r.link(link.first, link.second);
		}
		throw;
	}
}

std::size_t allocated()
{
	return index().size();
}

}
}
