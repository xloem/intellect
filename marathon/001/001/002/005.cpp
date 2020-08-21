#include "node-001-021.hpp"

#include <iostream>

int main()
{
	symbol(one);
	symbol(two);
	symbol(three);

	ref symbols = seq({one, two, three});

	ref encountered = create();
	for (ref sym = symbols; sym->isthing(); sym = sym->get(symbols::next)) {
		encountered->set(sym);
		for (ref sym2 = symbols; sym2->isthing(); sym2 = sym2->get(symbols::next)) {
			if (encountered->get(sym2)->isthing()) { continue; }
			encountered->set(sym2);
			std::cout << to_text(sym, false) << ", " << to_text(sym2, false) << std::endl;
			encountered->wipe(sym2);
		}
		encountered->wipe(sym);
	}
}
