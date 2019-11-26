#pragma once

#include "ref.hpp"
#include "../level-1/helpers.hpp"

namespace intellect {
namespace level2 {

// get a named variable
ref avariable(ref name);

template <typename... T>
ref and(T... refs)
{
	std::initializer_list<ref> rs = { refs... };
	decl(and, topic);
	ref ret = a(and);
	ref name; int count = 0;
	for (auto r : rs) {
		ret.link(topic, r);
		if (count == 0) {
			name.ptr() = r.ptr();
		} else {
			name.ptr() = (name-and-r).ptr();
		}
		++ count;
	}
	if (count == 1) {
		throw std::invalid_argument("and needs at least two subjects");
	}
	// in level-1, we'll want to name anonymous objects after defining them
	return name = ret;
}

// make a reference to a link
ref link(ref sourceref, ref typeref, ref targetref);

// invert a meaning
ref not(ref whatref);

}
}
