#pragma once

#include "reference.hpp"
#include "unique-data.hpp"

#include <library/string.hpp>

DECLARE name;

// interface proposal please
// 	-> propose making factory functions for creation vs access.
//
// thoughts on naming existing concepts, and merging[/splitting?] concepts?
// 	-> [LATER for splitting/merging] [thank you!!! oujr behavior is being labeled contributing now.  tiny.  but contributing =)]
// 	-> [naming can have method]

class named-thought : public reference
{
public:
	static named-thought find(std::initialier_list<library::string> specifiers);
	static named-thought create-named(std::initializer_list<library::string> specifiers);
	static named-thought create-unnamed(std::initializer_list<library::string> specifiers);

	void name(reference what);
	void associate(reference with);
	// first specifier is name
	// second specifier resolves synonyms etc
	// must be at least 2
private:
	using reference::reference;
	named-thought(std::initializer_list<library::string> specifiers);
	named-thought(reference source, std::initializer_list<library::string> specifiers);
};
