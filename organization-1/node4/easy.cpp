#include "easy.hpp"

#include <library/string.hpp>
#include <string>

using string = library::string;

std::unordered_map<std::string, reference> words;

// concepts: unique_data
//           operator form
//           kindedness

easy::easy(reference source)
: reference(source)
{
	data-default<library::string>(data-default<std::string>(data-default<library::string>("<unnamed>").std()));
}

easy::easy(library::string text)
{
	if (words.count(text.std())) {
		(*this) = words[text.std()];
	} else {
		data-default<library::string>(text);
		data-default<std::string>(text.std());
		words[text.std()] = *this;
	}
}
easy::easy(char const * text)
: easy(text)
{ }

easy & easy::operator=(easy & other)
{
	return (*this)= (reference &)other;
}

#include <unordered_map>
using kinded = std::unordered_map<reference, reference>

// can we relate around just-do-it and set-prep-to-do-it-a-year-later
// 	i think the pieces of just-do-it could relate with the parts of
// 	the plans they fit in with.  so, either work on the releevnt plan,
// 	or do it a way that doesn't require reimplementation.  maybe?
// 		let's implement easy in ways that are reusable elsewhere,
// 		at least a smidge.

easy & easy::operatr[](easy index)
{
	return 
	//return kind-get(index);
	kind-get returns a copy, makes assignment hard
}

METHOD void easy::write()
{
	library::stdout::write(self.data<library::string>());
}

METHOD void easy::line()
{
	library::stdout::line(self.data<library::string>());
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
