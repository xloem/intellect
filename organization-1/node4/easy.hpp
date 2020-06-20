#pragma once

#include "reference.hpp"
#include <library/string.hpp>

class easy-text : public reference
{
public:
	easy-text(reference source);
	easy-text(library::string text);
	using reference::operator=;

	METHOD void write();
	METHOD void line();

//	METHOD void apply
};

// sometimes we have singletons.  an object with a lot of methods.
// wonder how to make that.
struct input : public reference
{
	input(reference source);
	METHOD easy-text word();
	METHOD easy-text line();
};

//class easy-way : public reference
//{
//	easy-way(reference(*function)(reference focus, reference way));
//};
