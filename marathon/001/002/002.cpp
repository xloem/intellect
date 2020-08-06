#include "node-001-021.hpp"

#include <iostream>

int main()
{
	ref to_try = seq({symbols::hello, symbols::world, symbols::inputs, symbols::outputs, symbols::variable, symbols::state});

	std::cout << to_text(to_try) << std::endl;
}
