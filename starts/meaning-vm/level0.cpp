#include "level-0/level-0.hpp"

#include <iostream>

using namespace intellect::level0;

int main()
{
	ref a = alloc();
	ref b = alloc();
	ref c = alloc();
	ref d = alloc();
	ref e = alloc();

	ref skip = alloc();
	
	a->link(b, c);
	a->link(d, e);
	e->link(b, a);
	c->link(b, e);

	std::cout << a.dump(skip, skip) << std::endl;

	return 0;
}
