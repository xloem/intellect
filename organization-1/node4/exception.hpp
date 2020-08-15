#pragma once

#include "reference.hpp"

namespace sugar {

class exception : public reference, public std::logic_error
{
public:
	DECLARE exception-type;

	exception(reference (*type)(), char const * message);
};

}
