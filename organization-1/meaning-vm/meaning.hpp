#pragma once

#include "concept.hpp"
#include "helpers.hpp"

// get a named variable
ref avariable(ref name);

template <typename... T>
ref and(T... refs)
{
	std::initializer_list<ref> rs = { refs... };
	declrefs(and);
	decllnks(what);
	ref ret = a(and);
	ref name; int count = 0;
	for (auto r : rs) {
		ret[what] = r;
		if (count == 0) {
			name.ptr = r.ptr;
		} else {
			name.ptr = (name-and-r).ptr;
		}
		++ count;
	}
	if (count == 1) {
		throw std::logic_error("and needs at least two subjects");
	}
	return name = ret;
}

// make a reference to a link
ref link(ref sourceref, ref typeref, ref targetref);

// invert a meaning
ref not(ref whatref);
