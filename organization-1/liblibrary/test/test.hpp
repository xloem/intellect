#include <library/string.hpp>

using namespace library;

void worry(bool concern, string meaning)
{
	if (!concern) { return; }
	stderr::line(meaning);

	struct danger : public string{ using string::string; };
	throw danger(meaning.std());
}

// testvectors.cpp has a way to execute the same tests over multiple types
