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
