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
DECLARE reference-bool bool-false;
DECLARE reference-bool bool-true;

// maps.
// a map is just an ordered list of ways

// static-dependency template seems to provide some options.  could add contextual default methods to the idea.
	// both of these solutions seem like some work.  how would they go together?
		// with contextual default methods we no longer need static-dependency?
			// there'd be like a map of objects associated with each object.  this map is kinda the global default lookup
				// that sounds nice for other things too
			// the key here is to provide the map with a default function or something

// of-kind is a cool idea but it uses methods to perform static checks.
// 	should delay its checks until after reference's methods have been made.
// 		-> could register kind for construction
// 		-> could generate reference's method handlers with functions isntead of statics
// 		can still make static references
// 			^-- the second option above would provide the baseline fallbacks for everything
// 			but the order in which things are layered on top would still be deterministically random, different on different machines
// 				ideally we would specify that we need the methods of a particular thing to exist, before calling them.
// 				this is totally in the space of kind-of.
// 				but all these problems go away once the main function is called.
// 		-> we could use a contextual set of methods, instantiated in the main function or whoever produced us.
// 		-> we could make of-kind handle this by making sure somehow that methods for a kind exist before it is used.
// 				-> each kind would provide a function to be called to ensure initialization.  of-kind would call this
// 				   in the proper order using static function variable dependency.
// 			... okay that would work.  each class would pass its method constructor to of-kind using a template parameter.
// 			    of-kind could pvoide a function to ensure dependencies were instantiated
// 			but you'd want reference itself to be a kind, to ensure static dependency.

DECLARE reference old-implementation;
DECLARE reference current-implementation;

DECLARE reference is-of-kind;
DECLARE reference recognition-mistake;

// of-kind is a general reference subclass
template <reference&(* kind)()>
class of-kind : public reference
{
public:
	DECLARE reference is-of-kind;

	of-kind(reference const & other)
	: reference(other)
	{
		must-be(*this);
	}

	METHOD void construct() { } // can be called for construction: note inheritance unimplemented

	// for ininheritance we might eventually use a way of method-calling
	// this could be called from a call-operator reimplementation
	static void recognise-method(reference method-kind, reference basic-implementation, char const * classname, char const * methodname)
	{
		// TODO: we could make a little method-object that gets linked
		// it can show the old method from kind-get(method-kind) and the new method.
		// that would be more powerful if we made link objects: could link from the link
		if (kind().kind-get(method-kind) != null()) {
			throw presence-mistake;
		}
		kind().kind-set(method-kind, basic-implementation);
	}

	METHOD reference kind-get(reference property-kind)
	{
		// really what we want is, when placing a method, to provide a way to get the old method.

		// first check using reference::basic-kind-get
		reference result = reference::basic-kind-get()(self, property-kind);
		// if it is empty, then use our kind() object and get that
		if (result == null()) {
			result = kind().kind-get(property-kind);
		}
		return result;
	}

	static reference-bool & check-is(reference candidate)
	{
		if (candidate == null()) { return bool-false(); }
		return candidate.kind-get(kind()) == is-of-kind() ? bool-true() : bool-false();
	}

	static void ensure-is(reference member)
	{
		member.kind-set(kind(), is-of-kind());
	}

	static void must-be(reference member)
	{
		if (&check-is(member) == &bool-false()) {
			throw kindness-mistake();
		}
	}

protected:
	of-kind(any data = {})
	: reference(data)
	{
		kind-set(kind(), is-of-kind());
		kind-set(method-kind-get(), basic-kind-get());
		construct();
	}
};
DEFINE reference recognition-mistake;
template <reference&(* kind)()>
DEFINE reference of-kind<kind>::is-of-kind;


// class object seems to help.

class links-as-references : public reference
{
public:
//	METHOD(
};

// thinking of A) making quick definitions that are of kinds, when there
// are a lot of kinds.
// using an-object = of-kind<object>;
//
// DEFINE of-kind<object> fruit;
// using a-fruit = of-kind<fruit>
// AN object fruit
// A fruit apple
// DEFINE object fruit <- would be nice if object were of-kind<object>
//
// DEFINE object-group-kind
// DEFINE of-kind<object-group-kind> fruit
//
// DEFINE of-kind<fruit> apple;
// DEFINE of-kind<apple> red-apple;
// using fruit = of-kind<object>;
// using apple = of-kind<fruit>;
// DEFINE apple red-apple;


DEFINE reference kind-bool; // where does constexpr come from ?? [line numbering was shifted]
class reference-bool : private of-kind<kind-bool>
{
public:
	reference-bool(reference other)
	: of-kind(other)
	{
		if (other != bool-false() && other != bool-true())
		{
			throw kindness-mistake();
		}
	}

	reference-bool(bool value)
	: of-kind(value ? bool-true() : bool-false())
	{ }

	operator bool()
	{
		return (*this) == bool-true();
	}

	static reference-bool & check-is(reference other)
	{
		return (other == bool-true() || other == bool-false()) ? bool-true() : bool-false();
	}

	static void must-be(reference other)
	{
		if (check-is(other) == bool-false()) {
			throw kindness-mistake();
		}
	}

private:
	reference-bool(bool value, bool* token_for_instance_construction)
	: of-kind(value)
	{ }
	DECLARE reference-bool bool-true;
	DECLARE reference-bool bool-false;
};


DEFINE reference kind-connection-objects;
class connection-objects : public of-kind<kind-connection-objects>
{
};

DEFINE reference-bool reference-bool::bool-true(true, (bool*)0);
DEFINE reference-bool reference-bool::bool-false(false, (bool*)0);
DEFINE reference-bool bool-true(true);
DEFINE reference-bool bool-false(false);

// map would be a good candidate for being used before declared.
// it would be great to be able to index any reference with a map.
// it sounds like it might be a naturally higher-level construct, though.
// we can add the functionality to a global indexing function after the map is made.
// 	maybe maps can say they offer a way of indexing,
// 	and indexer can ask passed-index if it has a way.

// uh we got orderedness right enough now
// and it's totally reasonable to treat every ordered list as a map, already.

// next: provide map-lookup as core method

// okay, map-lookup has issue: ordered items can't look up by kind
// if the kind is a number O_O
// 	i guess you could provide a special attribute on the map entry
// 	to clear up whether it was a kind or a number
// 		if it was a kind that would change all the kinds
// 		or we could use entry objects
// 	it sounds like if it is a kind, it's obviously the kind
// 	otherwise you could interpret it as a number
// 	it kinda sounds like kind-get could do order lookup that
// 	way too.
// this makes a lookup that tries multiple things.
// seems it would be good to register those as ways-to-try, somehow, just
// an idea unmentioned.
// 	well ways-to-try sounds important .. but could easily be
// 	done later ...
// 		here: it would an order of ways.
// 		just like a map.
// 	okay, so we can call a method with a map.
// 		[and if our links were valued we could make time-value decisions
// 		 with a map]
// 	so the order that these improvements are laid upon each other has meaning
// 		we'd want the method implementation to let us set order
// [atm we are neither consciously planning nor implementing.
//  propose either implement or nap.]
//  	[thinking of consciously considering order of adding-stuff being
//  	 flexible.  seems just a little bit to wayness would be needed.]
//  	 	[flexibility requested was what?]
//  	 		[time-value decisions with map ..
//  	 		] [trust-us: is better to nap or implement.]
//  	 		=S ...................................
//

// okay, so options for handling maps that can map graphs with maps in them
// 	we could have a method for handling the map set
// 	the default one assumes integer steps are ordered, others
// 	are kinds
//
// 		this method is roughly kind-get

// let's offer methods

DEFINE reference kind-map-maker;
class map-maker : public of-kind<kind-map-maker>
{
public:
	DECLARE reference location;
	DECLARE reference map;

	static map-maker make(reference start) // be good if start could be a map
	{
		map-maker result = {};
		result.kind-set(location(), start);
		result.kind-set(map(), {});
		return result;
	}

	using of-kind<kind-map-maker>::of-kind;

	// todo: turn into method?
	void add(reference way)
	{
		reference map = kind-get(map-maker::map());
		map.order-set(map.order-count(), way);
	}


	// a way of building a map is starting at a spot,
	// and then moving in directions and seeing where you end up.
	
	// paired with a map is a log of where you _did_ end up.
	// this can be offered by indexing-by-maps and preservation of relevent
	// parts.
};
DEFINE reference map-maker::location;
DEFINE reference map-maker::map;

// time-value.
// we use connections that have value to them.
// this means we have connection objects.

// okay, so when we get a property, we return the connection object?
// when we bracket we return the connection object?
// 	when we get a property we get the destination f the connection object.

// all we need is some kind-options that are option types
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

class connection-reference : public reference
{
public:
	//connection-reference
};

class indirect-reference : public reference
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
class type-reference : public reference
{
public:
	// we want to be able to set members that are inherited by types
	// other possibilities include:
	// 	- creating something of a type
	// 	- verifying something is of a type
	// 	- verifying something of a type is used as that type should be used
};

// so.  we can now add a value to our links.
// let's just do it.  we can adjust internal structure later.

using degree_t = double;

DEFINE reference kind-degree;
class degree : public of-kind<kind-degree>
{
public:
	// TODO in real world: don't reject groups that have the attributes but not the label
	using of-kind<kind-degree>::of-kind;
	degree(degree_t value)
	{
		data-default<degree_t>() = value;
	}

	operator degree_t & ()
	{
		return data-default<degree_t>();
	}
};

DEFINE reference kind-time-value-choices("time-value-choices");
class time-value-choices : public of-kind<kind-time-value-choices>
{
	using data-type = std::unordered_map<reference, degree_t>;
public:
	using of-kind<kind-time-value-choices>::of-kind;

	time-value-choices()
	{
		data-default<data-type>();
	}

	// concepts associated with classes are stored as static functions
	// that return a reference.
	METHOD reference get-value(reference kind)
	{
		// note: focus is the method-equivalent 'this' [renamed self]
		return self.time-values()[kind];
	}

	// for implementing classes we have options:
	// 	- we could use a model object
	// 	- we could place defaults on the types and names
	// it seems it would be a kind-of thing.

	//static reference get-value(){static reference get-value((function<reference(reference)>)[](time-value-choices focus, reference kind) -> degree
	//{
	//	return {focus.time-values()[kind]};
	//}); return get-value;}

	//DEFINE(reference,,method-kind-get-value)

	// let's set it up with methods

	//degree get-value(reference kind)
	//{
	//	// this specifically gets the value of a kind
	//	return (*this)(method-kind-get-value(), {kind}); 
	//}

private:
	data-type & time-values()
	{
		return reference::data<data-type>();
	}
	
};// */

int main(int argc, char ** argv)
{
	reference concept, is, object, chair, wood(string("tree-body")), material;
	chair.kind-set(is, object);
	concept.kind-set(is, chair);
	concept.kind-set(material, wood);

	cout << (string)concept.kind-get(material).data<string>() << endl;
}