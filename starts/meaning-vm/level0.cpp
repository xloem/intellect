#include "level-0/level-0.hpp"

#include <functional>
#include <iostream>

using namespace intellect::level0;

#define out(name) std::cout << " " #name ":" << (long)name.ptr()

int main()
{
	ref store = alloc(concepts::allocations()); out(store);
	ref a = alloc(store); out(a);
	ref b = alloc(store); out(b);
	ref c = alloc(store); out(c);
	ref d = alloc(store); out(d);
	ref e = alloc(store); out(e);
	auto numlink = alloc(a); out(numlink);
	auto codelink = alloc(a); out(codelink);

	ref skip = alloc(store); out(skip);
	std::cout << std::endl;
	
	a.link(
		b, c,
		d, e
	);
	e.set(b, a);
	c.link(b, e);
	a.vset<int>(numlink, 3);
	a.vset<std::function<void()>>(codelink, [](){
		std::cout << "Hello, world." << std::endl;
	});

	std::cout << "Num: " << a.get(numlink).dump(skip, skip);
	std::cout << "Code: " << a.get(codelink).dump(skip, skip);
	std::cout << a.dump(skip, skip);
	std::cout << "Num: " << a.vget<int>(numlink) << std::endl;
	std::cout << "Code:  "; a.vget<std::function<void()>>(codelink)();

	std::cout << allocated() << " allocated" << std::endl;

	e.unlink(b, a);
	//dealloc(a, store);
	dealloc(store, concepts::allocations());

	std::cout << allocated() << " allocated" << std::endl;

	return 0;
}
