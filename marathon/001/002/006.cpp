#include "node-001-022.hpp"

#include <iostream>

int main()
{
	sym(one);
	sym(two);
	sym(three);

	ref symbols = seq({one, two, three});

	ref encountered = create();
	for (ref symbol = symbols; symbol->isthing(); symbol = symbol->get(symbols::next)) {
		encountered->set(symbol);
		for (ref symbol2 = symbols; symbol2->isthing(); symbol2 = symbol2->get(symbols::next)) {
			if (encountered->get(symbol2)->isthing()) { continue; }
			encountered->set(symbol2);
			std::cout << to_text(symbol, false) << ", " << to_text(symbol2, false) << std::endl;
			encountered->wipe(symbol2);
		}
		encountered->wipe(symbol);
	}
}
