#include "ref.hpp"
#include "seq.hpp"

int main()
{
	sym(one);
	sym(two);
	sym(three);
	seq list({one,two,three});

	cxxhabit printlist({},{syms::seq},[](ref context)
	{

	});
}
