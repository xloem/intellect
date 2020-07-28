#pragma once

#include "reference.hpp"
#include "unique-data.hpp"
#include "basic-assignables.hpp"

#include <library/string.hpp>

class easy : public unique-data<library::string>
{
public:
	easy(reference source);
	easy(const char * text);
	easy(library::string text);
	using reference::operator=;
	easy & operator=(easy & other);

	kinded-assignable operator[](easy kind);
	// now, returning the kinded-assignable is very similar to returning
	// a reference: but it makes sure to call the methods when used.

	operator const char *();

	METHOD void write();
	METHOD void line();

//	METHOD void apply
};

easy operator+(easy const & left, const char * right);
easy operator+(const char * left, easy const & right);

easy operator "" _e(const char*);

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
