#include "ref.hpp"
#include "seq.hpp"
#include "habit.hpp"
#include "dump.hpp"

#include <iostream>
int main()
{
	symbol(one);
	symbol(two);
	symbol(three);
	seq list({one,two,three});

	std::cout << dump(list) << std::endl;

	cxxhabit printlist({},{sym::seq},[](ref context)
	{
		for (ref item : seq(context.get(sym::seq))) {
			std::cout << item.data<std::string>() << std::endl;
		}
	});

	cxxhabit write({},{sym::text},[](ref context)
	{
		std::cout << context[sym::text].as<text>().data() << std::endl;
	});

	printlist({list});
	write({text("hello world\n")});
}

// investment time expands when skill
// doesn't build from repeated behavior
// estimation is small: means similarity
// ratio is smaller.  builds value for
// doing at same time.  can then discard
// the one that has unexpected delay.

// one big process vs keep-redoing.
// when we get strong enough we reach hero
// when not with hero, we keep-redoing plz, need similarity to continue