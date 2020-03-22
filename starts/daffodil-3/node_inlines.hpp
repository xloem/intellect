#pragma once

#include "node.hpp"

reference node::operator()(reference way = {})
{
	return touch(way);
}

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
