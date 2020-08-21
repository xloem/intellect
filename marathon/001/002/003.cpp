#include "node-001-021.hpp"

#include <iostream>

int main()
{
	symbol(wormhole);
	symbol(reality);
	symbol(mind);
	symbol(community);
	symbol(neuron);
	symbol(planet);
	symbol(atom);

	ref to_try = seq({wormhole, reality, mind, community, neuron, planet, atom});

	std::cout << to_text(to_try) << std::endl;
}
