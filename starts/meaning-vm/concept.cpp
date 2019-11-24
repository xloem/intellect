#include "concept.hpp"

ref concept::id()
{
	return this;
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

ref concept::get(ref const & type, bool quick) const
{
	// this is called by name(), so it passes quick=true
	auto result = links.equal_range(type.ptr);
	if (result.first == result.second) {
		if (quick) {
			throw std::out_of_range("no such concept link to get");
		} else {
			throw std::out_of_range("no such concept link to get: " + (std::string)type.name());
		}
	}
	return result.first->second;
}

concept::array concept::getAll(ref const & type) const
{
	array ret;
	for (
		auto range = links.equal_range(type.ptr);
		range.first != range.second;
		++ range.first
	) { 
		ret.emplace_back(range.first->second);
	}
	return ret;
}

void concept::link(ref const & type, ref const & target)
{
	links.insert({type.ptr, target.ptr});
}

void concept::unlink(ref const & type, ref const & target)
{
	auto ls = links.equal_range(type.ptr);
	for (auto l = ls.first; l != ls.second; ++ l) {
		if (l->second == target.ptr) {
			links.erase(l);
			return;
		}
	}
	throw std::out_of_range("no such concept link to erase");
}

void concept::unlink(ref const & type)
{
	auto ls = links.equal_range(type.ptr);
	if (ls.first == ls.second) {
		throw std::out_of_range("no such concept link to erase");
	}
	auto mid = ls.first;
	++ mid;
	if (mid != ls.second) {
		throw std::out_of_range("more than one link of type to erase");
	}
	links.erase(ls.first);
}
