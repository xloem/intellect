#include "level-1/level-1.hpp"

#include <iostream>

using namespace intellect::level1;
using namespace intellect::level1::concepts;

int main()
{
	decls(make, linked, habit);
	decls(needs, assumes, makes);
	decls(not, topic);
	decls(A, B, C);
	decls(source, type, target);
	decls(structure, function, argument, position);
	decls(variable, provide);

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

	std::cout << (make-linked).dump("dumped", true) << std::endl;

	return 0;
}
