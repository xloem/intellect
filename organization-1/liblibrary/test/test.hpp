#include <library/string.hpp>

#include <cmath>

using namespace library;

void worry(bool concern, string meaning)
{
	if (!concern) { return; }
	stderr::line(meaning);

	struct danger : public string{ using string::string; };
	throw danger(meaning.std());
}

bool differ_more_than(double a, double b, double within)
{
	return std::fabs(a - b) > within;
}

// testvectors.cpp has a way to execute the same tests over multiple types
