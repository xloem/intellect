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
	vref<int> num(3);
	vref<std::function<void()>> code([](){
		std::cout << "Hello, world." << std::endl;
	});
	auto numlink = alloc();
	auto codelink = alloc();

	ref skip = alloc();
	
	a.set(b, c);
	a.link(d, e);
	e.link(b, a);
	c.link(b, e);
	a.link(numlink, num);
	a.link(codelink, code);

	std::cout << "Num: " << ref(num).dump(skip, skip);
	std::cout << "Code: " << ref(code).dump(skip, skip);
	std::cout << a.dump(skip, skip);
	std::cout << "Num: " << a.vget<int>(numlink).val() << std::endl;
	std::cout << "Code:  "; a.vget<std::function<void()>>(codelink).val()();

	std::cout << allocated() << " allocated" << std::endl;

	e.unlink(b, a);
	dealloc(a);
	dealloc(c);
	dealloc(e);
	dealloc(b);
	dealloc(d);
	dealloc(numlink);
	dealloc(codelink);
	dealloc(num);
	dealloc(code);
	dealloc(skip);

	std::cout << allocated() << " allocated" << std::endl;

	return 0;
}
