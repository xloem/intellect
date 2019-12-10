#include "level-1/level-1.hpp"

#include <iostream>

using namespace intellect::level1;
using namespace intellect::level1::concepts;

int main()
{
	std::cout << intellect::level0::allocated() << " allocated" << std::endl;

	decls(make, linked, habit);
	std::cout << intellect::level0::allocated() << " allocated" << std::endl;
	decls(needs, assumes, makes);
	decls(not, topic);
	decls(A, B, C);
	decls(source, type, target);
	decls(structure, function, argument, position);
	decls(variable, provide);
	decls(act);

	(make-linked).link(
		is, habit,
		needs, a(structure).link(
			is, function-argument,
			argument-position, ref(1),
			a(variable, A), provide,
			a(variable, B), provide,
			a(variable, C), provide
		)
	);
	movetoname(a(link), A-B-C-linked).link(
		link-source, A,
		link-type, B,
		link-target, C
	);
	a(not, not-A-B-C-linked).set(topic, A-B-C-linked);
	(make-linked).link(
		assumes, not-A-B-C-linked,
		makes, A-B-C-linked
	);
	(make-linked).fset(
		act,
		(std::function<void(ref)>)[&](ref args)
		{
			ref source = args[A];
			ref type = args[B];
			ref target = args[C];
			std::cout << "Linking " << source.name() << " by " << type.name() << " to " << target.name() << std::endl;
			source.link(type, target);
		}
	);

	std::cout << (make-linked).dump("dumped", true) << std::endl;

	decls(apple, fruit);
	(make-linked)[act]
		(a(function-argument)
		 .link(
			 A, apple,
			 B, is,
			 C, fruit
		));

	std::cout << apple.dump("dumped", true) << std::endl;

	std::cout << intellect::level0::allocated() << " allocated" << std::endl;
	intellect::level0::dealloc(intellect::level0::concepts::allocations(), intellect::level0::concepts::allocations());
	std::cout << intellect::level0::allocated() << " allocated" << std::endl;

	return 0;
}
