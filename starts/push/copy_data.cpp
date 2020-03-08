#include "loaded_relevance.hpp"

void copy_data(Context & context)
{
	// change this however you like, was quick to make sure the output would run
	data[context.relevance["output"]] = data[context.relevance["input"]];
	context.relevance["behavior"] = context.relevance["output"];
	run(context);
}
