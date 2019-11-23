#include "concept.hpp"

ref concept::id()
{
	return this;
}

bool concept::linked(ref type)
{
	return links.count(type.ptr) > 0;
}

bool concept::linked(ref type, ref target)
{
	for (ref t : getAll(type)) {
		if (t == target) {
			return true;
		}
	}
	return false;
}

ref concept::get(ref type)
{
	auto result = links.equal_range(type.ptr);
	if (result.first == result.second) {
		throw std::out_of_range("no such concept link to get: " + type.name());
	}
	return result.first->second;
}

concept::array concept::getAll(ref type)
{
	array ret;
	for (
		auto range = links.equal_range(type.ptr);
		range.first != range.second;
		++ range.first
	) { 
		ret.push_back(range.first->second);
	}
	return ret;
}

void concept::link(ref type, ref target)
{
	links.insert({type.ptr, target.ptr});
}

void concept::unlink(ref type, ref target)
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
