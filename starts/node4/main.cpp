#include <iostream>
#include <string>

#include "reference.hpp"

using namespace std;

// okay time-value, maybe maps

// been thinking on this a lot.  we've been finding ways around every situation.
// just considering: say I wanted a boolean kind, and also to return
// the boolean kinds in the kind class.
// 	this may work but may lead you to compilation errors.
// 	let's make this compile and try it.

// maps.
// a map is just an ordered list of ways

template <reference & kind>
class of-kind : public reference
{
public:
	static reference is-of-kind;

	of-kind()
	: reference()
	{
		set(kind, is-of-kind);
	}

	of-kind(reference const & other)
	: reference(other)
	{
		if (*this != null) {
			must-be();
		}
	}

	constexpr operator reference&() { return *this; }

	static bool check-is(reference candidate)
	{
		if (candidate == null) { return false; }
		return candidate.get(kind) == is-of-kind;
	}

	static void ensure-is(reference member)
	{
		member.set(kind, is-of-kind);
	}

	void must-be()
	{
		if (!check-is(*this)) {
			throw kindness_mistake;
		}
	}

public:
};

reference kind-map;

class map : public of-kind<kind-map>
{
public:
	static reference home;

	// both a copy constructor and initializes a map to a home.
	// using this makes it impossible to make a map of a map,
	// so CHANGE APPROACH
	map(reference start)
	: of-kind<kind-map>(check-is(start) ? start : null)
	{
		if (!check-is(start)) {
			ensure-is(*this);
			set(home, start);
		}
	};

	// a way of building a map is starting at a spot,
	// and then moving in directions and seeing where you end up.
	
	// paired with a map is a log of where you _did_ end up.
	// this can be offered by indexing-by-maps and preservation of relevent
	// parts.
};

// time-value.
// we use connections that have value to them.
// this means we have connection objects.

// okay, so when we get a property, we return the connection object?
// when we bracket we return the connection object?
// 	when we get a property we get the destination f the connection object.

// all we need is some kind_options that are option types
// an option type has a value and a reference

// concept: set-of-choices
// kind: choice

// concept: choice
// datatype: double (time-value)
// ...

// let's make a hierarchical getter .. oh that would mean isness

/*
reference _get((function<reference(reference,reference)>)[](reference focus, reference kind) -> reference
{
	
});
*/




//
// basics:
// 	wayness: how-to-update, how-to-pick
// 	activate state
// 	connectedness: time-values on choices

// choices will have values on each option
// so implementing connectedness a little could help.
	// connectedness could use some basics of typedness.

// we note that gettersetterness has great utility for generalization
// we also note that connectedness aids time-value
// 	the generalized solution is to use wayness for gettersetterness.  then wayness can do connectedness and typedness and others.
// 		the original wayness needs some kind of typedness.  this will be handled by simply copying it around.

// we're proposing there is a special member for getting attributes.  this member can implement typedness, connectedness, etc.

// we just need words for theese things.

class connection_reference : public reference
{
public:
	//connection_reference
};

class indirect_reference : public reference
{
	// say we want to make [] = work.
	// it would help to use connection objects.
};

/*
class getter : public reference
{
public:
	static reference getter;

	reference operator[](reference which);
};
*/

// this could be merged into the root concept: it would check for a get member.

// possibility
class type_reference : public reference
{
public:
	// we want to be able to set members that are inherited by types
	// other possibilities include:
	// 	- creating something of a type
	// 	- verifying something is of a type
	// 	- verifying something of a type is used as that type should be used
};

int main(int argc, char ** argv)
{
	reference concept, is, object, chair, wood, material;
	chair.set(is, object);
	concept.set(is, chair);
	concept.set(material, wood);

	((string&)wood) = "tree-body";

	cout << (string)concept.get(material) << endl;
}
