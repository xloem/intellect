#include "level-0/level-0.hpp"

#include <functional>
#include <iostream>

using namespace intellect::level0;

#define out(name) std::cout << " " #name ":" << std::hex << (size_t)name.ptr() << std::dec

int main()
{
	std::cout << allocated() << " allocated" << std::endl;

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
	
	a.get(codelink).setcrucial();
	try {
		dealloc(a.get(codelink), a);
		throw "deallocd crucial concept";
	} catch (crucial_concept & e) {
		realloc(a.get(codelink), concepts::level0allocations());
	}
	a.setcrucial(codelink, a.get(codelink));
	a.setcrucial(numlink, a.get(numlink));
	try {
		a.unlink(codelink);
		throw "unlinkd crucial link";
	} catch (crucial_link_type_target & e) {
		realloc(a, concepts::level0allocations());
		realloc(codelink, concepts::level0allocations());
		realloc(numlink, concepts::level0allocations());
	}

	for (auto c : { a, a.get(codelink), a.get(numlink) } )
	for (auto it = c.links().begin(); it != c.links().end();) {
		if (!c.crucial(it) && !it->first.linked(concepts::allocator(), concepts::level0allocations())) {
			c.unlink(it++);
		} else {
			++ it;
		}
	}

	e.unlink(b, a);
	//dealloc(a, store);
	dealloc(store, concepts::allocations());

	std::cout << allocated() << " allocated" << std::endl;

	std::cout << "=== (feel free to move the below to its most effective spot) ===" << std::endl;
	std::cout << "Please support Karl working on this intelligence framework." << std::endl;
      	std::cout << "The framework needs to be quality, so that the rest may function." << std::endl;
	std::cout << "=== === === === === === === ===  === === === === === === === ===" << std::endl;

	return 0;
}
