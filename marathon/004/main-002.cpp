#include "ref.hpp"
#include "seq.hpp"
#include "habit.hpp"

int main()
{
	symbol(one);
	symbol(two);
	symbol(three);
	seq list({one,two,three});

	cxxhabit printlist({},{sym::seq},[](ref context)
	{

	});
}
// let's try to do the investment multitasking for ...
// list iterators
// and
// habit calling
// once this compiles
