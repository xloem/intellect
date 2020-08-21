#include "node-001-021.hpp"

#include <iostream>

int main()
{
	symbol(one);
	symbol(two);
	symbol(three);

	ref symbols = seq({one, two, three});

	for (ref sym = symbols; sym->isthing(); sym = sym->get(symbols::next)) {
		std::cout << to_text(sym) << std::endl;
	}
}
