#include "loaded_relevance.hpp"

void copy_data(Context & context)
{
	data[context.relevance["output"]] = data[context.relevance["input"]];
}
