#pragma once

namespace expressions {

// Syntactical operator details
struct syntax_operator
{
	enum syntax_operator_identifier identifier;
	enum syntax_operator_fixing type;
	char const * symbol;
	char const * name;
};

extern syntax_operator const * operators; // one for each identifier

} // namespace expressions
