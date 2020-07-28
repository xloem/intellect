#include "easy.hpp"

#include <library/string.hpp>

using string = library::string;
using library::operator""_s;

// concepts: unique_data
//           operator form
//           kindedness

easy::easy(reference source)
: unique-data("<unnamed "_s + std::hash<reference>()(source) + ">"_s, source)
{ }

easy::easy(string text)
: unique-data(text)
{ }
easy::easy(char const * text)
: easy(string(text))
{ }

easy & easy::operator=(easy & other)
{
	(*(reference*)this) = (reference &)other;
	return *this;
}

//#include <unordered_map>
//using kinded = std::unordered_map<reference, reference>

// can we relate around just-do-it and set-prep-to-do-it-a-year-later
// 	i think the pieces of just-do-it could relate with the parts of
// 	the plans they fit in with.  so, either work on the releevnt plan,
// 	or do it a way that doesn't require reimplementation.  maybe?
// 		let's implement easy in ways that are reusable elsewhere,
// 		at least a smidge.

kinded-assignable easy::operator[](easy index)
{
	//return kind-get(index);
	//kind-get returns a copy, makes assignment hard
	return kinded-assignable(*this, index);
}

METHOD void easy::write()
{
	library::stdout::write(self.data());
}

METHOD void easy::line()
{
	library::stdout::line(self.data());
}

easy operator "" _e(const char * text)
{
	return text;
}

input::input(reference source)
: reference(source)
{ }

METHOD easy input::word()
{
	return library::stdin::word();
}

METHOD easy input::line()
{
	return library::stdin::line();
}
