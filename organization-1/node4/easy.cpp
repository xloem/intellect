#include "easy.hpp"

#include <library/string.hpp>
#include <string>

using string = library::string;

std::unordered_map<std::string, reference> words;

easy-text::easy-text(reference source)
: reference(source)
{
	data-default<library::string>(data-default<std::string>(data-default<library::string>("<unnamed>").std()));
}

easy-text::easy-text(library::string text)
{
	if (words.count(text.std())) {
		(*this) = words[text.std()];
	} else {
		data-default<library::string>(text);
		data-default<std::string>(text.std());
		words[text.std()] = *this;
	}
}

METHOD void easy-text::write()
{
	library::stdout::write(self.data<library::string>());
}

METHOD void easy-text::line()
{
	library::stdout::line(self.data<library::string>());
}

input::input(reference source)
: reference(source)
{ }

METHOD easy-text input::word()
{
	return library::stdin::word();
}

METHOD easy-text input::line()
{
	return library::stdin::line();
}
