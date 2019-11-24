#include "concept.hpp"
#include "errors.hpp"

using namespace intellect::level0;

#define selfref const_cast<concept*>(&self)

ref concept::id()
{
	return this;
}

void concept::link(ref const & type, ref const & target)
{
	links.insert({type.ptr, target.ptr});
}

void concept::unlink(ref const & type, ref const & target)
{
	auto ls = links.equal_range(type);
	for (auto l = ls.first; l != ls.second; ++ l) {
		if (l->second == target) {
			links.erase(l);
			return;
		}
	}
	throw no_such_link_type_target(selfref, type, target);
}

void concept::unlink(ref const & type)
{
	auto ls = links.equal_range(type);
	if (ls.first == ls.second) {
		throw no_such_link_type(selfref, type);
	}
	auto mid = ls.first;
	++ mid;
	if (mid != ls.second) {
		throw link_type_not_unique(selfref, type);
	}
	links.erase(ls.first);
}

bool concept::linked(ref const & type) const
{
	return links.count(type.ptr) > 0;
}

bool concept::linked(ref const & type, ref const & target) const
{
	for (ref const & t : getAll(type)) {
		if (t == target) {
			return true;
		}
	}
	return false;
}

concept::array concept::getAll(ref const & type) const
{
	array ret;
	for (
		auto range = links.equal_range(type);
		range.first != range.second;
		++ range.first
	) { 
		ret.emplace_back(range.first->second);
	}
	return ret;
}

ref concept::get(ref const & type) const
{
	auto result = links.equal_range(type.ptr);
	if (result.first == result.second) {
		throw no_such_link_type(selfref, type);
	}
	auto test = result.first;
	++ test;
	if (test != result.second) {
		throw link_type_not_unique(selfref, type);
	}
	return result.first->second;
}

void concept::set(ref const & type, ref const & target)
{
	if (linked(type)) {
		throw link_type_not_unique(selfref, type);
	}
	link(type, target);
}


