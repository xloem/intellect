#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace tools {

std::string next_line_word()
{
	// inhibition inside this behavior.
		// any coments?
	
	static std::istringstream words;

	std::string line;
	if !(std::getline(std::cin, line)) { return {}; }
	
	std::istringstream words(line);
	std::string word;
}

}
