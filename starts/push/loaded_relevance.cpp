
#include "loaded_relevance.hpp"

// Use relevence to make code that runs, out of our own instructions.

// We will set up a quick relevence structure that just copies the code into memory and runs it.

std::unordered_map<std::string, Data> data;
std::unordered_map<std::string, Context> contexts;

Context & Context::peer(char const *name)
{
	return contexts[name];
}

Data & Context::peer_data(char const *name)
{
	return data[name];
}

/*
std::string & Context::get(char const *name)
{
	return relevance[name];
}
*/
