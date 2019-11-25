#include "concept.hpp"
#include "errors.hpp"

using namespace intellect::level0;

#define selfref const_cast<concept*>(&self)

concept* concept::id()
{
	return this;
}

void concept::link(concept* type, concept* target)
{
	links.insert({type, target});
}

void concept::unlink(concept* type, concept* target)
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

void concept::unlink(concept* type)
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

bool concept::linked(concept* type) const
{
	return links.count(type) > 0;
}

bool concept::linked(concept* type, concept* target) const
{
	for (concept* t : getAll(type)) {
		if (t == target) {
			return true;
		}
	}
	return false;
}

concept::array concept::getAll(concept* type) const
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

concept* concept::get(concept* type) const
{
	auto result = links.equal_range(type);
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

void concept::set(concept* type, concept* target)
{
	if (linked(type)) {
		throw link_type_not_unique(selfref, type);
	}
	link(type, target);
}


