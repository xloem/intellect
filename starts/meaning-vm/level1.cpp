#include "level-1/level-1.hpp"

#include <iostream>

using namespace intellect::level1;
using namespace intellect::level1::concepts;

int main()
{
	decl(make, linked, habit);
	decl(needs, assumes, makes);
	decl(not, topic);
	decl(A, B, C);
	decl(source, type, target);

	(make-linked).set(is, habit);
	(make-linked).set(needs, []() -> ref {
			decl(structure, function, argument, position);
			decl(variable, A, B, C, provide);

			ref ret = a(structure);
			ret.link(is, function-argument);
			ret.set(argument-position, ref(1)),
			ret.set(a(variable, A), provide);
			ret.set(a(variable, B), provide);
			ret.set(a(variable, C), provide);
			return ret;
	}());
	a(link, A-B-C-linked).set(link-source, A);
	(A-B-C-linked).set(link-type, B);
	(A-B-C-linked).set(link-target, C);
	a(not, not-A-B-C-linked).set(topic, A-B-C-linked);
	(make-linked).set(assumes, not-A-B-C-linked);
	(make-linked).set(makes, A-B-C-linked);

	std::cout << (make-linked).dump("dumped", true) << std::endl;

	return 0;
}
