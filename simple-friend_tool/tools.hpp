#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace tools {

bool starts_with(std::string const & word, std::string const & start)
{
	return word.compare(0, start.size(), start) == 0;
}

char const * without_start(std::string const & word, std::string const & start)
{
	return word.c_str() + start.size();
}

std::string mutate_start_into_endfixes(std::string const & word, char const * word_prefix, char const * word_suffix, std::string const & start, std::string const & start_prefix, char const * start_suffix)
{
	if (starts_with(word, start)) {
		return start_prefix + without_start(word, start) + start_suffix;
	} else {
		return word_prefix + word + word_suffix;
	}
}

std::string quote_special(std::string const & word, bool quotes = true)
{
	return mutate_start_into_endfixes(word, quotes ? "'" : "", quotes ? "'" : "", "special-", "[", "]");
}

std::string line_word()
{
	// this part stores a line-expression, and provides the words
	// from the line in order.
	
	static std::istringstream words;
	static bool newline = true;

	if (newline) {
		std::string line;
		if (!std::getline(std::cin, line)) { return {}; }

		words.str(line);
		words.clear();
		newline = false;
	}
	
	std::string word;
	if (!(words >> word)) {
		newline = true;
		return {};
	}

	return word;
}

}
