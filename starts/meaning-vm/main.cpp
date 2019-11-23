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
	A[is = variable];
	B[is = variable];
	C[is = variable];

	// add a new unique link to a concept
	// given A, B, C
	// and assuming A is not linked by B to C,
	// makes A be linked by B to C.
	
	// would like to do A = a(variable);
	// 	solution might be to tag anons as such, and take them with a condition in operator=
	// since we want to return a ref, change [is=variable] to use refs.  this means learning
	// to delete them, which means checking if they are used or not.
	// 	where do we put the deleting?
	// 	right now helpers is doing memory.  maybe instead we can have a memory class.
	// 	ideally memory is handled by a concept.
	// 	let's make a pool of concepts?
	(add-link-unique)[
		is = habit,
		needs = A, needs = B, needs = C,
		makes = (abc-linked)[is = link, link-source = A, link-type = B, link-target = C],
		assumes = (abc-not-linked)[is = trueness, what = abc-linked, truth = false]
	];

	dumpconcept(add-link-unique);
}
