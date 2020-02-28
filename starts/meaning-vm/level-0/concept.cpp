#include "concept.hpp"
#include "errors.hpp"

using namespace intellect::level0;

#define selfref const_cast<concept*>(&self)

concept::concept()
: _refcount(0), iscrucial(false)
{ }

concept::~concept() noexcept(false)
{
	if (refcount() != 0) { throw still_referenced(this); }
	for (auto it = links.cbegin(); it != links.cend();) {
		it = unlink(it);
	}
}

concept* concept::id()
{
	return this;
}

void concept::link(concept* type, concept* target)
{
	if (type == 0 || target == 0) { throw null_reference(); }
	if (type != this) { ++ type->_refcount; }
	if (target != this) { ++ target->_refcount; }
	links.insert({type, target});
}

bool concept::crucial(concept* type, concept* target)
{
	auto ls = links.equal_range(type);
	bool wascrucial = false;
	bool wasnotcrucial = false;
	for (auto l = ls.first; l != ls.second; ++ l) {
		if (l->second == target) {
			if (crucialparts.count(l)) { wascrucial = true; }
			else { wasnotcrucial = true; }
		}
	}
	if (wascrucial && wasnotcrucial) { throw link_type_not_unique(selfref, type); }
	if ((!wascrucial) && (!wasnotcrucial)) { throw no_such_link_type(selfref, type); }
	return wascrucial;
}

void concept::setcrucial(concept* type, concept* target)
{
	auto ls = links.equal_range(type);
	for (auto l = ls.first; l != ls.second; ++ l) {
		if (l->second == target) {
			if (!crucialparts.count(l)) {
				setcrucial(l);
				return;
			}
		}
	}
	throw no_such_link_type(selfref, type);
}

void concept::replace(concept* other)
{
	for (auto it = links.cbegin(); it != links.cend();) {
		it = unlink(it);
	}
	links = other->links;
	data = other->data;
	for (auto & link : links) {
		if (link.first != this) { ++ link.first->_refcount; }
		if (link.second != this) { ++ link.second->_refcount; }
	}
}

void concept::unlink(concept* type, concept* target)
{
	auto ls = links.equal_range(type);
	bool wascrucial = false;
	for (auto l = ls.first; l != ls.second; ++ l) {
		if (l->second == target) {
			if (crucialparts.count(l)) { wascrucial = true; continue; }
			links.erase(l);
			if (type != this) { -- type->_refcount; }
			if (target != this) { -- target->_refcount; }
			return;
		}
	}
	if (wascrucial) { throw crucial_link_type_target(selfref, type, target); }
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
	unlink(ls.first);
}

void concept::relink(concept::linkit it, concept* target)
{
	if (target == 0) { throw null_reference(); }
	if (crucialparts.count(it)) {
		throw crucial_link_type_target(selfref, it->first, it->second);
	}
	if (it->second != this) { -- it->second->_refcount; }
	if (target != this) { ++ target->_refcount; }
	// calling erase on an empty range returns a mutable iterator without changing the container
	links.erase(it, it)->second = target;
}

concept::linkit concept::unlink(concept::linkit it)
{
	if (crucialparts.count(it)) {
		throw crucial_link_type_target(selfref, it->first, it->second);
	}
	if (it->first != this) { -- it->first->_refcount; }
	if (it->second != this) { -- it->second->_refcount; }
	return links.erase(it);
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
	auto range = links.equal_range(type);
	return array(range.first, range.second);
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
		unlink(type);
	}
	link(type, target);
}


