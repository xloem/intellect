#pragma once

#include "ref.hpp"

class assignable : public ref
{
private:
	// if this were a link object, it may need to be unique-data
	// additionally the unordered_map structure doesn't have a way
	// to persistently identify the same link.  we would need a
	// way to differentiate identical links, somehow.
	// this class could manage that, of course.  it could decide
	// first-in-bucket or second-in-bucket.  give an index of
	// identical link.
	assignable(ref source, basic_link & source) {
	}
};
