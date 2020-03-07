#include "loaded_relevance.hpp"

void run(Context & context)
{
	void(*func)(Context&) = (void(*)(Context&))(data[context.relevance["behavior"]].data());
	func(context);
}
