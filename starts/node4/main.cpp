#include <iostream>
#include <string>

#include "reference.hpp"

using namespace std;

// okay time-value, maybe maps

// seems valuable to register subclasses for construction via some initialization
// method.  this lets us significantly reduce complexity-recursion by detecting
// uninitialized references and using fallback methods.

// been thinking on this a lot.  we've been finding ways around every situation.
// just considering: say I wanted a boolean kind, and also to return
// the boolean kinds in the kind class.
// 	this may work but may lead you to compilation errors.
// 	let's make this compile and try it.
// 		two issues are encountered
// 		1: unsure around having reference-methods return reference-bool.  could make casting issue.  guess both would be a good idea?
// 		2, more significant: returning reference-bool means defining the function that does so, after reference-bool.
// 			2 is solvable? oh no it sn't ... because it's return-by-value.  we could return-by-reference though.
//
// 			okay, so this solution ended up using differing code in one of the dependency classes.
// 			seems a little messy .... if we do that a lot the code will become difficult to maintain
// 				things can't be reordered without a lot of recoding, maybe.
// 					i guess we wouldn't used kind-bool until we have kinds.
//
// 					we could normalize using subclasses of references, somehow.
//
// 						1. we want to define the references using the subclass, so that its constructor is called
// 						2. we want to access the references using the subclass, _if_ we have encountered the subclass
// 							^-- this could be mostly fixed by method-kinds
// 						3. we want to be able to use the references as references, before the subclass has been encountered.
// 					so a solution might be for each subclass-reference to have two forms: a root-class form and a subclass form.
// 					the root-class form is a reference to it.
// 						but how do we accept and return these subclasses from member functions, compile-time?
// 						casting operators are methods ...
// 							options:
// 								- templates, force the cast
// 								- pointers
// 								- let reference construct from anything else and convert to anything else (clashes with data impl,
// 								  can resolve by removing casting operator, adds clarity)
// 								  	we'd want to make data constructors use factories, i suppose
// 								  	this takes more typing but works.  when new typed objects are creaed you go
// 								  	type object = type::make(stuff)
// 								  	previously you'd go
// 								  	type object(stuff);
// 								  		that's like *140% but probably doesn't happen that much
// 				thinking on letting subclasses be used before
// 				declared.  to reduce complexity-recursion.
// 					static-definition-issue.
// 					if a parent class tris to make use of
// 					the child class in any way before main
// 					function, system crashes 
// 						hmm we don't really want to do that
// 						it happens with operators and
// 						stuff maybe
// 					it'll happen in constructors of subclasses.
// 						good point.
// 					you can work around by making static objects be static functions that return them -- but you can still get into recursion loops in these functions.  still the problem is roughly unavoidable and should probably be solved.
// 				well atm we do have fallback methods ... they are called for undefined pointers only if the pointer value is 0 before being defined, which is likely toolchain-specific.
// 					hierarchy can make sense.  but it's interesting that this is solvable if we can double-construct.
// 					for example, the constructor of a subclass could register the subclass in a global table
// 					then they could all be constructed after being initialied to null.
// 					something to think about.  stick to hierarchy for now.  finish struts if desired.
// okay the idea is that the of-kind members use references instead of bools
// and bools are a kind with only two members

class reference-bool;
extern reference-bool & bool-false;
extern reference-bool & bool-true;

// maps.
// a map is just an ordered list of ways

template <reference & kind>
class of-kind : public reference
{
public:
	static reference is-of-kind;

	of-kind(reference const & other)
	: reference(other)
	{
		must-be(*this);
	}

	constexpr operator reference&() { return *this; }

	static reference-bool & check-is(reference candidate)
	{
		if (candidate == null) { return bool-false; }
		return candidate.kind_get(kind) == is-of-kind ? bool-true : bool-false;
	}

	static void ensure-is(reference member)
	{
		member.kind_set(kind, is-of-kind);
	}

	static void must-be(reference member)
	{
		if (&check-is(member) == &bool-false) {
			throw kindness_mistake;
		}
	}

protected:
	of-kind(any data = {})
	: reference(data)
	{
		kind_set(kind, is-of-kind);
	}
};

reference kind-bool;
class reference-bool : private of-kind<kind-bool>
{
public:
	reference-bool(reference other)
	: of-kind(other)
	{
		if (other != bool-false && other != bool-true)
		{
			throw kindness_mistake;
		}
	};

	reference-bool(bool value)
	: of-kind(value ? bool-true : bool-false)
	{ }

	operator bool()
	{
		return (*this) == bool-true;
	}

	static reference-bool & check-is(reference other)
	{
		return (other == bool-true || other == bool-false) ? bool-true : bool-false;
	}

	static void must-be(reference other)
	{
		if (check-is(other) == bool-false) {
			throw kindness_mistake;
		}
	}

private:
	reference-bool(bool value, bool* token_for_instance_construction)
	: of-kind(value)
	{ }
	static reference-bool bool-true;
	static reference-bool bool-false;
};


reference kind-connection-objects;
class connection-objects : public of-kind<kind-connection-objects>
{
};

reference-bool reference-bool::bool-true(true, (bool*)0);
reference-bool reference-bool::bool-false(false, (bool*)0);

// map would be a good candidate for being used before declared.
// it would be great to be able to index any reference with a map.
// it sounds like it might be a naturally higher-level construct, though.
// we can add the functionality to a global indexing function after the map is made.
// 	maybe maps can say they offer a way of indexing,
// 	and indexer can ask passed-index if it has a way.

// uh we got orderedness right enough now
// and it's totally reasonable to treat every ordered list as a map, already.

// next: provide map-lookup as core method

// let's offer methods
reference kind-map-maker;
class map-maker : public of-kind<kind-map-maker>
{
public:
	static reference location;

	static map make(reference start)
	{
		map result = {};
		map.kind-set(last-location, start);
	}

	using of-kind<kind-map>;

	// todo: turn into method?
	void add(reference way)
	{
		order_set(order_count(), way);
	}


	// a way of building a map is starting at a spot,
	// and then moving in directions and seeing where you end up.
	
	// paired with a map is a log of where you _did_ end up.
	// this can be offered by indexing-by-maps and preservation of relevent
	// parts.
private:
	map()
	: of-kind<kind-map>()
	{ }
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
