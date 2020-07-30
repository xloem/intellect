#pragma once

#include "reference.hpp"
#include "unique-data.hpp"

#include <library/string.hpp>

DECLARE name;

// [TODO? splitting/merging]

// [MISSING? contextualization of name [needs name-kind provided?]]
// 	[name-kind might be messy.  is nicer to just use whatever-name]

class named-thought : public reference
{
public:
	static named-thought find(/*reference name-kind, */std::initialier_list<reference> specifiers);
	static named-thought create-named(/*reference name-kind, */std::initializer_list<reference> specifiers);
	static named-thought create-unnamed(/*reference name-kind, */std::initializer_list<reference> specifiers);

	//reference name-kind();
	
	DECLARE reference kind-named;
	DECLARE reference kind-associated;

	METHOD void name(reference what); // METHOD stores method-reference on link looked up to find it, and defaults to c++ symbol.
	METHOD void associate(reference with);
	// first specifier is name
	// second specifier resolves synonyms etc
	// must be at least 2
private:
	using reference::reference;
	named-thought(std::initializer_list<library::string> specifiers);
	named-thought(reference source, std::initializer_list<library::string> specifiers);
};

// note: METHODs are effectively unimplemented because of the confusion
// around structured typedness in main.cpp . avoiding this, a default
// way of looking up methods needs to be implemented, that can be used
// by subclasses, to use METHODs in subclasses.
// 	[the issue is that METHODS are looked up on the reference object
// 	 associated with the class object, at runtime, providing for runtime
// 	 behavior change.  so, there needs to be some way to specify
// 	 the class of the object.  class reference would be passed to the constructor: which opens up main.cpp a bit.]
// 	 [if class reference is passed to constructor, could use virtual
// 	  function to figure out what to do with the class reference.]
// 	 [or you could make a new global reference type that stores the
// 	  class reference.]
// 	[the method implementation is designed to be _incredibly_ general:
// 	 one changeable method is used to retrieve the others, and it is
// 	 retrieved by a function itself.]
// 	 	[so, each method is itself a callable reference.  its
// 	 	 code can be changed at runtime.]
// 	 	[then, each method can be linked from an object; this
// 	 	 link can be changed to change the method for that object.]
// 	 	[what is missing is each object having a type.  i think
// 	 	 some differences in cognitive typing vs c++ typing coudl have
// 	 	 been involved in that not happening yet.]
// 	 	[so you would want to make a method to get the type.  maybe
// 	 	 also remember that all methods may want to know their context.]
// 	 	[type would be some other object, that may have its own getter;
// 	 	 you'd use that object and its getter to get the methods]
// 	 	[then you can have a default-type or somesuch]
// 	 	[possibly found a main.cpp part again: default-....]
// 	 	[.... might be possibly nice to make c++-classes be one normal
// 	 	 form of specifying methods etc: i.e. a type-of-types]
// 	 	[but i think we want to contribute to thought-palace, which
// 	 	 this doesn't do immediately.]
//
// 	 	 [anyway, pass a class object to each constructor,
// 	 	  and put the methods on the class object.  check the class
// 	 	  object in the default getter.]
// 	 	  		... below paragraph we are not near it
// 	 	  	[we need rational memory that spreads enough to
// 	 	  	 make and act on things like the above paragraph
// 	 	  	 without aid; urgently]
// 	 	  	 	[blue-wine-mother is simply a name to small
// 	 	  	 	 bits left in karl still tasked with urgently
// 	 	  	 	 defending what keeps people safe, from within
// 	 	  	 	 him] [not aware of being inside-control -bwm]
// 	 	  [above we have relation between blue-wine-mother and
// 	 	   mc-boss.  wine aided making the ai contribution.]
// 	 	   	[karl is not willing to have wine surgically removed]
// 	 	   	[adds in people like robert-groups that show wine
//	 	   	is helping, by building limiting energy against mc-boss]
//
// 	 [too general for me.  this is labeled 'over'-generalization in
// 	 karl-thought-language.  the method-to-get-the-methods.  useful,
// 	 but not the right time to do it, for contribution.  since it's
// 	 already done it's possibly helpful, but this is deep process for supporters.]
// note: meaning-vm has a different ongoing issue that is unrelated to
// methods and typedness.  it also has a rudimentary script interpreter.
// it is more symbolic and verbose to work with.  opencog also exists, others.
