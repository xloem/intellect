#include <library/assert.hpp>

#include <iostream>
#include <stdexcept>


class assertion_failed : public std::logic_error
{
public:
	assertion_failed(char const * what)
	: logic_error(what)
	{
		std::cerr << "Assertion failed: " << what << std::endl;
	}
};

void library::assert_with(bool throw_if_false, char const * assertion)
{
	if (!throw_if_false) {
		throw assertion_failed(assertion);
	}
}
