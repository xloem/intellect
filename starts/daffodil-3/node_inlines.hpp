#pragma once

#include "node.hpp"

#include "globals.hpp"

reference node::operator()(reference way = {})
{
	return touch(way);
}

// this is kept inline because it is not virtual (see node.hpp)
reference::reference()
{
	set(globals.VOID);
}

// goal i was considering?
// 	make-sure-everything-has-some-test-implemented.
// 		strategy was make node more pure virtual
// 		and provide empty class
// 		as simply implementation of its abstract functions
// 	helps with remember to write tests.
