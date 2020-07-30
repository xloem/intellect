#pragma once

// Just a note: this easy.hpp file is obviously a quick attempt to make
// something usable, and results of it might be imported by a more
// advanced project

#include "reference.hpp"
#include "unique-data.hpp"
#include "easy-assignables.hpp"

#include <library/string.hpp>

// can you let us make thoughts with non-unique names?
// can you let us identify if a thought has a name?
// 	-> one approach is to use ["name"] to be the name.
// 		[how get all thoughts with a name?]
// 			[could use e.g. ordered links on the name]
//
// we would like an interface where you can get all the thoughts given a name,
// and possibly also given a thought get its name
// 	[is solved by walking links reverse-wise]
// 	[this is crazy! well meaning-vm supports multiple-links-one-type]
// 	[here we have to make a link to an ordered list? or something?]
// 		[okay actually reviewed this, and proposed reference with
// 		ordered content was fine for multiple links =S]
class easy : public unique-data<library::string>
{
public:
	easy(reference source);
	easy(const char * text);
	easy(library::string text);
	using reference::operator=;
	easy & operator=(easy & other);

	inverting-kinded-assignable operator[](easy kind);
	// now, returning the kinded-assignable is very similar to returning
	// a reference: but it makes sure to call the kind-get or kind-set methods when used.

	METHOD void write();
	METHOD void line();

//	METHOD void apply
};

// multiple could be used to track incoming links
// you could iterate over the links incoming, using it
// i wonder if inverting-kinded-assignable coudl be changed to use it?
// looks like it would work for link objects?
class multiple : public reference
{
public:
	using reference::reference;

	ordered-assignable operator[](index_t index);
};

//easy operator-(easy const & left, easy const & right);
//easy operator+(easy const & left, easy const & right);

easy operator "" _e(const char text [], unsigned long size);

// sometimes we have singletons.  an object with a lot of methods.
// wonder how to make that.
struct input : public reference
{
	input(reference source);
	METHOD easy word();
	METHOD easy line();
};

//class easy-way : public reference
//{
//	easy-way(reference(*function)(reference focus, reference way));
//};
