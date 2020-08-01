#include <library/string.hpp>

#include <cmath>

#include <stdexcept>

using namespace library;

struct danger : public std::logic_error { using logic_error::logic_error; };

void worry(bool concern, string meaning)
{
	if (!concern) { return; }
	throw danger(meaning.std());
}

bool differ_more_than(double a, double b, double within)
{
	return std::fabs(a - b) > within;
}

// note: testvectors.cpp has a way to execute the same tests over multiple types

bool called_default = false;
bool called_copy = false;
bool called_assign = false;
bool called_destroy = false;
class call_tracker
{
public:
	call_tracker()
	{
		called_default = true;
	}

	call_tracker(call_tracker const &)
	{
		called_copy = true;
	}

	call_tracker& operator=(call_tracker const &)
	{
		called_assign = true;
		return *this;
	}

	~call_tracker()
	{
		called_destroy = true;
	}
};

