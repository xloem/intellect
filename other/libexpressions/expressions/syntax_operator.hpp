#pragma once

//namespace library { class string; }
#include <library/string.hpp>

#include "syntax_operator_identifier.hpp"

namespace expressions {

struct syntax_operator
{
	enum fixing {
		prefix,
		infix,
		postfix,
		subscript
	};
	syntax_operator_identifier identifier;
	fixing type;
	library::string symbol;
	library::string name;
};

extern syntax_operator const operators[syntax_operators_count];

} // namespace expressions
