
// we want to try sequences of symbols
// and output them, roughly

#include "node-001-021.hpp"

//////////
// TEST //
//////////

using namespace symbols;

#include <iostream>
int main()
{
	ref to_try = seq({hello, world, inputs, outputs, variable, state});

	// algorithmic expansion, internal.
	
	std::cout << to_text(to_try);
}
