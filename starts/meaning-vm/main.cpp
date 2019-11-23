#include "concept.hpp"
#include "helpers.hpp"

using namespace std;

#include <iostream>

void dumpconcept(ref r)
{
	declrefs(dumped, name);

	for (auto & l : r->links) {
		if (ref(l.first) == name) {
			continue;
		}
		cout << r << " " << ref(l.first) << " " << ref(l.second) << endl;
	}
	if (!r->linked(dumped)) {
		r[dumped = true];
	}
	for (auto & l : r->links) {
		if (!l.second->linked(dumped)) {
			dumpconcept(l.second);
		}
	}
}

int main()
{
	declrefs(is, link, type);
	is->link(is, link-type);

	declrefs(source, target);
	declrefs(linked, A, B, C, abc, variable);
	declrefs(trueness, truth, what, not);

	declrefs(add, unique, habit, needs, assumes, makes);
	A = a(variable);
	B = a(variable);
	C = a(variable);

	// add a new unique link to a concept
	// given A, B, C
	// and assuming A is not linked by B to C,
	// makes A be linked by B to C.
	add-link-unique = a(habit)[
		needs = a(variable, A), needs = a(variable, B), needs = a(variable, C),
		makes = a(link, abc-linked)[link-source = A, link-type = B, link-target = C],
		assumes = a(trueness, abc-not-linked)[what = abc-linked, truth = false]
	];

	dumpconcept(add-link-unique);
}
