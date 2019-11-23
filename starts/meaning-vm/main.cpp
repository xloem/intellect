#include "concept.hpp"
#include "helpers.hpp"
#include "meaning.hpp"

using namespace std;

#include <iostream>

void dumpconcept(ref r)
{
	decllnks(dumped, name);

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
	declrefs(make, linked, habit);
	declrefs(A, B, C);
	decllnks(needs, assumes, makes);

	// add a new unique link to a concept
	// given A, B, C
	// and assuming A is not linked by B to C,
	// makes A be linked by B to C.
	// NEXT? make code for make-linked that takes a ref
	//       change the needs structure to use a model for the ref,
	//       with needed values specified as 'provided'
	make-linked = a(habit)[
		needs = and(avariable(A), avariable(B), avariable(C)),
		assumes = not(A-B-C-linked = link(A, B, C)),
		makes = A-B-C-linked
	];

	dumpconcept(make-linked);
}
