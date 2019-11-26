#include "level-0/level-0.hpp"

#include <functional>
#include <iostream>

using namespace intellect::level0;

int main()
{
	ref a = alloc();
	ref b = alloc();
	ref c = alloc();
	ref d = alloc();
	ref e = alloc();
	auto numlink = alloc();
	auto codelink = alloc();

	ref skip = alloc();
	
	a.set(b, c);
	a.link(d, e);
	e.link(b, a);
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
	auto num = a.get(numlink);
	auto code = a.get(codelink);
	dealloc(a);
	dealloc(num);
	dealloc(code);
	dealloc(numlink);
	dealloc(codelink);
	dealloc(c);
	dealloc(e);
	dealloc(b);
	dealloc(d);
	dealloc(skip);

	std::cout << allocated() << " allocated" << std::endl;

	return 0;
}
