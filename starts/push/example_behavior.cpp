#include "loaded_relevance.hpp"

#include <stdio.h>
void example_behavior(Context & context)
{
	decltype(&printf)(context.peer_data("system-printf").data())("Hello, world.\n");
};
