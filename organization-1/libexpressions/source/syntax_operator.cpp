#include <expressinos/syntax_operator.hpp>
#include <expressions/syntax_operator_identifier.hpp>
#include <expressions/syntax_operator_fixing.hpp>

namespace expressions {

extern syntax_operator const operators_storage[syntax_operators_count];

syntax_operator const * operators = operators_storage;

syntax_operator const operators_storage[syntax_operators_count] =
{
	{plus, infix, "+", "plus"},
	{minus, infix, "-", "minus"},
	{negate, prefix, "-", "minus"},
	{multiplies, infix, "*", "multiplies"},
	{divides, infix, "/", "divides"},
	{modulus, infix, "%", "modulus"},
	{bit_xor, infix, "^", "bit_xor"},
	{bit_and, infix, "&", "bit_and"},
	{bit_or, infix, "|", "bit_or"},
	{bit_not, prefix, "~", "bit_not"},
	{logical_not, prefix, "!", "logical_not"},
	{assign, infix, "=", "assign"},
	{less, infix, "<", "less"},
	{greater, infix, ">", "greater"},
	{assign_plus, infix, "+=", "assign_plus"},
	{assign_minus, infix, "-=", "assign_minus"},
	{assign_multiplies, infix, "*=", "assign_multiplies"},
	{assign_divides, infix, "/=", "assign_divides"},
	{assign_modulus, infix, "%=", "assign_modulus"},
	{assign_bit_xor, infix, "^=", "assign_bit_xor"},
	{assign_bit_and, infix, "&=", "assign_bit_and"},
	{assign_bit_or, infix, "|=", "assign_bit_or"},
	{shift_left, infix, "<<", "shift_left"},
	{shift_right, infix, ">>", "shift_right"},
	{assign_shift_left, infix, "<<=", "assign_shift_left"},
	{assign_shift_right, infix, ">>=", "assign_shift_right"},
	{equal_to, infix, "==", "equal_to"},
	{not_equal_to, infix, "!=", "not_equal_to"},
	{less_equal, infix, "<=", "less_equal"},
	{greater_equal, infix, ">=", "greater_equal"},
	{logical_and, infix, "&&", "logical_and"},
	{logical_or, infix, "||", "logical_or"},
	{preincrement, prefix, "--", "preincrement"},
	{predecrement, prefix, "--", "predecrement"},
	{postincrement, postfix, "++", "postincrement"},
	{postdecrement, postfix, "++", "postdecrement"},
	{comma, infix, ",", "comma"},
	{star, prefix, "*", "star"},
	{arrow_star, postfix, "->*", "arrow_star"},
	{arrow, postfix, "->", "arrow"},
	{index, bracketing, "[]", "index"},
	{call, bracketing, "()", "call"},
	{three_way, infix, "<=>", "three_way"},
	{cast, prefix, "(cast)", "cast"},
	{literal, postfix, "<literal>", "literal"},
	{syntax_operators_count, ~0, "", "syntax_operators_count"},
};

}
