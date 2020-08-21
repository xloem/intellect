#pragma once

#include "ref.hpp"

class assignable : public ref
{
public:
	// if this were a link object, it may need to be unique-data
	// additionally the unordered_map structure doesn't have a way
	// to persistently identify the same link.  we would need a
	// way to differentiate identical links, somehow.
	// this class could manage that, of course.  it could decide
	// first-in-bucket or second-in-bucket.  give an index of
	// identical link.
	assignable(ref source, basic_link const & link);
	assignable(ref source, ref type);

	assignable& operator=(ref const & other);

private:
	ref type;
	bool linked;
	basic_link link;
};

assignable::assignable(ref source, basic_link const & link)
: ref(source), type(link->first), linked(true), link(link)
{ } 

assignable::assignable(ref source, ref type)
: ref(source), type(type), linked(false)
{ } 

assignable& assignable::operator=(ref const & other)
{
	if (linked) {
		link->second = other;
	} else {
		link = set(type, other);
		linked = true;
	}
	return *this;
}

assignable ref::operator[](ref const & what)
{
	auto iterator = (**this).refs.find(what);
	if (iterator != (**this).refs.end()) {
		return {*this, (**this).refs.find(what)};
	} else {
		return {*this, what};
	}
}
