#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace tools {

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
