#define main_unit

#include "ref.hpp"
#include "assignable.hpp"
#include "dump.hpp"
#include "exception.hpp"

#include <iostream>
int main()
{
	ref hi({{sym::what, sym::is}});
	hi[sym::what] = sym::what;
	std::cout << dump(hi) << std::endl;
}
