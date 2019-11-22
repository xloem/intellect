#include "concept.hpp"
#include "helpers.hpp"

using namespace std;

#include <iostream>

void dumpconcept(ref r)
{
	declrefs(dumped);

	for (auto & l : r->links) {
		cout << r << " " << l.first << " " << l.second << endl;
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
	A[is = variable];
	B[is = variable];
	C[is = variable];

	// add a new unique link to a concept
	// given A, B, C
	// and assuming A is not linked by B to C,
	// makes A be linked by B to C.
	(add-link-unique)[
		is = habit,
		needs = A, needs = B, needs = C,
		makes = (abc-linked)[is = link, link-source = A, link-type = B, link-target = C],
		assumes = (abc-not-linked)[is = trueness, what = abc-linked, truth = false]
	];

	dumpconcept(add-link-unique);
}
