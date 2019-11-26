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

	(make-linked).set(is, habit);
	(make-linked).set(needs, []() -> ref {
			decls(structure, function, argument, position);
			decls(variable, A, B, C, provide);

			ref ret = a(structure);
			ret.link(is, function-argument);
			ret.set(argument-position, ref(1)),
			ret.set(a(variable, A), provide);
			ret.set(a(variable, B), provide);
			ret.set(a(variable, C), provide);
			return ret;
	}());
	movetoname(a(link), A-B-C-linked).set(link-source, A);
	(A-B-C-linked).set(link-type, B);
	(A-B-C-linked).set(link-target, C);
	a(not, not-A-B-C-linked).set(topic, A-B-C-linked);
	(make-linked).set(assumes, not-A-B-C-linked);
	(make-linked).set(makes, A-B-C-linked);

	std::cout << (make-linked).dump("dumped", true) << std::endl;

	return 0;
}
