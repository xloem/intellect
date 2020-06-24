#pragma once

namespace expressions {

enum syntax_operator_identifier : unsigned char;
enum syntax_operator_fixing : unsigned char;

// Syntactical operator details
struct syntax_operator
{
	syntax_operator_identifier identifier;
	syntax_operator_fixing type;
	char const * symbol;
	char const * name;
};

extern syntax_operator const * operators; // one for each identifier

} // namespace expressions
