#include "exception.hpp"

#include <library/string.hpp>

using namespace library;

DEFINE sugar::exception::exception-type;

sugar::exception::exception(reference (*type)(), char const * message)
: reference(library::string(message)), std::logic_error(message)
{
	kind-set(exception-type(), type());
}
