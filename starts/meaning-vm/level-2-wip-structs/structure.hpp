#pragma once

#include "common.hpp"
#include "../level-1/helpers.hpp"

namespace intellect {
namespace level2 {

namespace concepts {
	static	decl(structure), // group for all structures
		decl(fill), // group for values to be filled in
		decl(topic), // references the structure under consideration
		decl(linked), decl(link), // describes a structure link
		decl(source), decl(type), decl(target), // link attributes
		decl(first), // for finding first of a list
		decl(next) // for iteration over list
		;
};

// make a structure
ref structure(ref name, std::initializer_list<ref> attributes);

// make an object of the given structure
ref structured(ref structure, std::initializer_list<ref> fills);

// throw if an object is not of the given structure
void muststructured(ref topic, ref structure);

}
}
